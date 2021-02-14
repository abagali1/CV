// Anup Bagali
// Period 7
// 02/12/2021

#include <bits/stdc++.h>

#define K 5
#define KD 2
#define X 800
#define Y 800

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
                d += pow(this->data[i-c.data[i]], 2);
            return sqrt(d);
        }
        const bool operator==(Comparable<T, N> &c) const {
            return this->data == c.data;
        }
        const bool operator!=(Comparable<T, N> &c) const {
            return this->data != c.data;
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

typedef unordered_map<Point, vector<Point>, PointHash> PointMap;
typedef array<array<int, 2>, 2> Bounds;


template<typename T>
class KDNode{
    static_assert(is_base_of<ComparableBase, T>::value);
    private:
        KDNode *_l, *_r;
        T _value;
        int bounds[KD][2];
        T possible_centroids[K];
        T centroid;
    public:
        KDNode(T val): _l(NULL), _r(NULL), _value(val) {};
        KDNode(T val, int b[KD][2]): _l(NULL), _r(NULL), _value(val){
            for(int i=0;i<KD;i++)
                for(int j=0;j<2;j++)
                    this->bounds[i][j] = b[i][j];
        }
        ~KDNode(){
            delete this->_l;
            delete this->_r;
        }
        KDNode<T>* get_left(){ return this->_l; }
        KDNode<T>* get_right(){ return this->_r; }
        T get_value(){ return this->_value; }
        void set_left(KDNode<T> *n){ this->_l = n; }
        void set_right(KDNode<T> *n){ this->_r = n; }
        void set_value(T val){this->_value = val; }

        static KDNode<T>* insert(KDNode<T> *root, T value, Bounds bounds, int d = 0){
            if(root == NULL)
                return new KDNode<T>(value, bounds);
            int i = d % KD;
            if(value[i] < root->get_value()[i]){
                b[i][0] = bounds[i][0];
                b[i][1] = root->get_value()[i];
                root->set_left(KDNode<T>::insert(root->get_left(), value, b, d+1));
            }else{
                b[i][0] = root->get_value()[i];
                b[i][1] = bounds[i][1];
                root->set_right(KDNode<T>::insert(root->get_right(), value, b, d+1));
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

int N = 50;
const Color BLACK({0, 0, 0});
const Color RED({255, 0, 0});
const Color BLUE({0, 0, 255});
const Color GREEN({0, 255, 0});
const Color PURPLE({255, 0, 255});

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
            fprintf(fout, "%d %d %d\n", colors[i][j][0], colors[i][j][1], colors[i][j][2]);
    fclose(fout);
}

// Point centroid_for_point(KDNode<Point> *root, Point centroids[K], Point p){

// }

bool reorganize(KDNode<Point> *root, PointMap &previous, Point centroids[K], vector<Point> &points){
    bool organized = true;
    for(int i=0;i<K;i++){
        double x = 0;
        double y = 0;
        int n = 0;
        for(const Point &p: previous[centroids[i]]){
            x += p[0];
            y += p[1];
            n++;
        }
        Point nc = Point({x/n, y/n});
        organized = organized && nc != centroids[i];
        centroids[i] = nc;
    }
    previous.clear();
    return organized;
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

    Point centroids[K];
    KDNode<Point> *tree = NULL;
    Bounds bounds = {{0, X}, {0, Y}};
    for(int i=0;i<N;i++)
        tree = KDNode<Point>::insert(tree, points[i], bounds);


    // PointMap organized_centroids;

    // bool organized = false;
    // do{
    //     organized = reorganize(tree, organized_centroids, centroids, points);

    // }while(!organized);
    


    // Color **colors = new Color*[X];
    // for(int i=0;i<X;i++)
    //     colors[i] = new Color[Y];
    

    // write_ppm(colors);
    
    // for(int i=0;i<X;i++)
    //     delete[] colors[i];
    // delete[] colors;

    delete tree;
}


int main(int argc, char* argv[]){
    srand(time(0));
    part4();
}
