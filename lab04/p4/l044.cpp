// Anup Bagali
// Period 7
// 02/12/2021

#include <bits/stdc++.h>

#define K 5
#define KD 2
#define X 800
#define Y 800

#define E 0.0001
#define INFILE "points.txt"
#define OUTFILE "clusters.ppm"

#define uchar unsigned char

using namespace std;

class ComparableBase{};

template<class T, int N>
class Comparable : public ComparableBase{
    public:
        array<T, N> data;
        Comparable(const T(&list)[N]){
            for(int i=0;i<N;i++)
                this->data[i] = list[i];
        };
        void print() const{
            for(int i=0;i<N;i++)
                printf("%lf ", this->data[i]);
            printf("\n");
        }
        const inline double distance(Comparable<T, N> &c) const {
            double d = 0;
            for(int i=0;i<N;i++)
                d += pow(this->data[i]-c.data[i], 2);
            return sqrt(d);
        }
        const bool operator==(Comparable<T, N> &c) const {
            return this->data == c.data;
        }
        const bool operator!=(Comparable<T, N> &c) const {
            for(int i=0;i<N;i++)
                if(this->data[i] != c.data[i])
                    return true;
            return false;
        }
        const T& operator[](int i) const {
            return this->data[i];
        }
};

class Point: public Comparable<double, 2>{
    public:
        Point(const double(&list)[2]): Comparable<double, 2>(list) {};
        Point(): Comparable<double, 2>({rand()/(double)RAND_MAX, rand()/(double)RAND_MAX}) {};
        Point(Point& p1, double scalar): Comparable<double, 2>({p1.data[0]*scalar, p1.data[1]*scalar}) {};
        Point(Point& p1, double s_x, double s_y): Comparable<double, 2>({p1.data[0]*s_x, p1.data[1]*s_y}) {};

        const bool operator==(const Point &p) const{
            return this->data[0] == p.data[0] && this->data[1] == p.data[1];
        }

        Point operator-(const Point &p) const{
            return Point({this->data[0]-p[0], this->data[1]-p[1]});
        }

        double dot(const Point &p) const{
            return this->data[0]*p[0] + this->data[1]*p[1];
        }

        static inline Point midpoint(Point p1, Point p2){
            return Point({(p1[0]+p2[0])/2, (p1[1]+p2[1])/2});
        }
};

class Line {
    public:
        double a, b, c;
    Line(double a0, double b0, double c0){
        a = a0;
        b = b0;
        c = c0;
    }
    Line(Point p1, Point p2){
        a = p2[1] - p1[1];
        b = p1[0] - p2[0];
        c = a*(p1[0]) + b*(p1[1]);
    }
    void print(){
        printf("A: %e B: %e, C: %e\n", this->a, this->b, this->c);
    }

    static Line perpendicular_bisector(Line l, Point p1, Point p2){
        Point mid = Point::midpoint(p1,p2);
        return Line(-l.b,l.a,-l.b*(mid[0])+l.a*(mid[1]));
    }
};

class PointHash{
    public:
        const size_t operator()(const Point &p) const {
            return hash<double>()(p[0]) ^ hash<double>()(p[1]);
        }
};

class Color: public Comparable<uchar, 3>{
    public:
        Color(): Comparable<uchar, 3>({255, 255, 255}) {};
        Color(const uchar(&list)[3]): Comparable<uchar, 3>(list) {};
};

template<typename T>
class KDNode{
    static_assert(is_base_of<ComparableBase, T>::value);
    private:
        KDNode *_l, *_r;
        T _value;
        T centroid;
    public:
        vector<T> possible_centroids = vector<T>(K);
    
        KDNode(T val): _l(NULL), _r(NULL), _value(val) {};
        ~KDNode(){
            delete this->_l;
            delete this->_r;
        }
        KDNode<T>* get_left(){ return this->_l; }
        KDNode<T>* get_right(){ return this->_r; }
        T get_value(){ return this->_value; }
        T get_centroid(){ return this->centroid; }
        void set_left(KDNode<T> *n){ this->_l = n; }
        void set_right(KDNode<T> *n){ this->_r = n; }
        void set_value(T val){this->_value = val; }
        void set_centroid(T c){this->centroid = c;}

        const bool leaf() const {
            return this->_l == NULL && this->_r == NULL;
        }

        static KDNode<T>* insert(KDNode<T> *root, T value, int d = 0){
            if(root == NULL)
                return new KDNode<T>(value);
            int i = d % KD;
            if(value[i] < root->get_value()[i]){
                root->set_left(KDNode<T>::insert(root->get_left(), value, d+1));
            }else{
                root->set_right(KDNode<T>::insert(root->get_right(), value, d+1));
            }
            return root;
        }

        static int height(KDNode<T> *n){
            if(n == NULL)
                return 0;
            int lh = KDNode<T>::height(n->get_left());
            int rh = KDNode<T>::height(n->get_right());
            return max(lh, rh) + 1;
        }
};

typedef array<array<double, 2>, 2>  Bounds;
typedef unordered_set<Point, PointHash> PointSet;
typedef unordered_map<Point, vector<Point>, PointHash> PointMap;

int N = 50;
const Color BLACK({0, 0, 0});
const Color RED({255, 0, 0});
const Color BLUE({0, 0, 255});
const Color GREEN({0, 255, 0});
const Color PURPLE({255, 0, 255});

const Bounds B0 = {{{0, 1}, {0, 1}}};

static inline void set_pixel(Color **c, double x, double y, Color color){
    if(x >= X || x < 0 || y >= Y || y < 0)
        return;
    c[(int)x][(int)y] = color;
}

static inline void set_pixel(Color **c, Point p, Color color){
    set_pixel(c, p[0], p[1], color);
}

void draw_circle(Color **c, Point center, double r, Color color){
    set_pixel(c, center, color);
    int x = 0;
    int y = r;
    int xmax = (int) (r * 0.70710678); 
    int y2 = y * y;
    int y2_new = y2;
    int ty = (2 * y) - 1;
    for (x = 0; x <= xmax; x++) {
        if ((y2 - y2_new) >= ty) {
            y2 -= ty;
            y -= 1;
            ty -= 2;
        }
        set_pixel (c, center[0]+x, center[1]+y, color);
        set_pixel (c, center[0]+x, center[1]-y, color);
        
        set_pixel (c, center[0]-x, center[1]+y, color);
        set_pixel (c, center[0]-x, center[1]-y, color);
        
        set_pixel (c, center[0]+y, center[1]+x, color);
        set_pixel (c, center[0]+y, center[1]-x, color);
        
        set_pixel (c, center[0]-y, center[1]+x, color);
        set_pixel (c, center[0]-y, center[1]-x, color);
        
        y2_new -= (2 * x) - 3;
    }
}

void write_points(vector<Point> &points){
    FILE* fout = fopen(INFILE, "w");
    for(Point &p: points)
        fprintf(fout, "%0.17lf %0.17lf\n", p[0], p[1]);
    fclose(fout);
}

void read_file(vector<Point> &points){
    points.clear();
    N = 0;
    ifstream fin (INFILE);
    string line;
    remove(OUTFILE);
    while(getline(fin, line)){
        int s = line.find(" ");
        points.push_back(Point({stod(line.substr(0, s), NULL), stod(line.substr(s+1,s), NULL)}));
        N++;
    }
}

void write_ppm(Color **colors){
    FILE* fout = fopen(OUTFILE, "w");
    fprintf(fout, "P3\n%d %d\n255\n", X, Y);
    for(int j=0;j<Y;j++)
        for(int i=0;i<X;i++)
            fprintf(fout, "%d %d %d\n", colors[i][Y-1-j][0], colors[i][Y-1-j][1], colors[i][Y-1-j][2]);
    fclose(fout);
}

void gen_palette(Color palette[K]){
    palette[0] = Color({128,128,128});
    palette[1] = Color({255, 0, 0});
    palette[2] = Color({0, 255, 0});
    palette[3] = Color({0, 0, 255});
    palette[4] = Color({255, 0, 255});
}

void print_bounds(Bounds b){
    printf("Bounds: %lf %lf %lf %lf\n", b[0][0], b[0][1], b[1][0], b[1][1]);
}

Point min_point(Point p1, vector<Point> &points, double *md = NULL){
    if(points.size() == 1)
        return points[0];
    double d = DBL_MAX;
    Point mp;
    for(const Point &p: points){
        double dist = p.distance(p1);
        if(dist < d){
            d = dist;
            mp = p;
        }
    }
    if(md != NULL)
        *md = d;
    return mp;
}

void get_boundary_points(Bounds b, Point points[4]){
    points[0] = Point({b[0][0], b[1][0]});
    points[1] = Point({b[0][0], b[1][1]});
    points[2] = Point({b[0][1], b[1][0]});
    points[3] = Point({b[0][1], b[1][1]});
}

bool dominates(Point p1, Point p2, Point bps[4]){ // does p1 dominate p2
    for(int i=0;i<4;i++)
        if(bps[i].distance(p2) < bps[i].distance(p1))
            return false;
    return true;
}

void exclude_centroids(Bounds b, vector<Point> &possible_centroids, vector<Point> &new_centroids){
    int n = possible_centroids.size();
    
    Point boundaries[4];
    get_boundary_points(b, boundaries);
    
    for(int i=0;i<n;i++){
        for(int j=0;j<n;j++){
            if(i != j){
                bool d = dominates(possible_centroids[i], possible_centroids[j], boundaries);
                if(d){
                    new_centroids.erase(remove(new_centroids.begin(), new_centroids.end(), possible_centroids[j]), new_centroids.end());
                }
            }
        }
    }
}

void update(KDNode<Point> *root, vector<Point> &possible_centroids, PointMap &centroids, Bounds bounds = B0, int d = 0){
    if(root == NULL)
        return;

    Point mc = min_point(root->get_value(), possible_centroids);
    root->set_centroid(mc);
    centroids[mc].push_back(root->get_value());

    vector<Point> new_centroids = possible_centroids;
    exclude_centroids(bounds, possible_centroids, new_centroids);

    int i = d % KD;
    Bounds lb, rb;
    lb[!i] = bounds[!i];
    rb[!i] = bounds[!i];
    lb[i] = {bounds[i][0], root->get_value()[i]};
    rb[i] = {root->get_value()[i], bounds[i][1]};
    
    
    update(root->get_left(), new_centroids, centroids, lb, d+1);
    update(root->get_right(), new_centroids, centroids, rb, d+1);
}

bool centroid_equals(Point c1, Point c2){
    return (abs(c1[0]-c2[0]) < E) && (abs(c1[1] - c2[1]) < E);
}

bool reorganize(PointMap prev, vector<Point> &prev_means){
    bool organized = true;
    for(int i=0;i<K;i++){
        double x = 0;
        double y = 0;
        int n = 0;
        for(const Point &p: prev[prev_means[i]]){
            x += p[0];
            y += p[1];
            n++;
        }
        Point nm({x/n, y/n});
        if(!centroid_equals(nm, prev_means[i])){
            organized = false;
        }
        prev_means[i] = nm;
    }
    if(!organized)
        prev.clear();
    return organized;
}

void pgl(KDNode<Point> *root, int d){
    if(root == NULL)
        return;
    if(d==1){
        root->get_centroid().print();
        root->get_value().print();
        cout << endl;
    }else if(d > 1){
        pgl(root->get_left(), d-1);
        pgl(root->get_right(), d-1);
    }
}

void part4(){
    string in;
    do{
        cout << "Generate Points? yes/no ";
        cin >> in;
    }while(in != "yes" && in != "no");
   
    vector<Point> points(N);
    if(in == "yes")
        write_points(points);
    else if(in == "no")
        read_file(points);

    vector<Point> centroids(K);
    PointMap organized_centroids;
    KDNode<Point> *tree = NULL;
    for(int i=0;i<N;i++)
        tree = KDNode<Point>::insert(tree, points[i]);

    bool organized = false;
    int i =0;
    do{
        update(tree, centroids, organized_centroids);
        organized = reorganize(organized_centroids, centroids);
        i++;
    }while(!organized);

    Color palette[K];
    gen_palette(palette);
    Color **colors = new Color*[X];
    for(int i=0;i<X;i++)
        colors[i] = new Color[Y];
    
    for(int i=0;i<K;i++){
        vector<Point> points = organized_centroids[centroids[i]];
        for(int j=0;j<points.size();j++)
            draw_circle(colors, Point(points[j], X, Y), 2.0, palette[i]);
        draw_circle(colors, Point(centroids[i], X, Y), 5.0, BLACK);
    }

    write_ppm(colors);
    
    for(int i=0;i<X;i++)
        delete[] colors[i];
    delete[] colors;
    delete tree;
}


int main(int argc, char* argv[]){
    srand(time(0));
    part4();
}
