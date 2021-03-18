// Anup Bagali
// Period 7
// 03/17/2021
// l043 REDO
#include <bits/stdc++.h>

#define uchar unsigned char

#define KD 2
#define X 800
#define Y 800
#define INFILE "points.txt"
#define OUTFILE "diagram.ppm"

using namespace std;

class Point {
    public:
        double x, y;
    Point(){
        this->x = (double)rand()/(double)RAND_MAX;
        this->y = (double)rand()/(double)RAND_MAX;
    }
    Point(double _x, double _y): x(_x), y(_y){};
    Point(Point p1, double scalar): x(p1.x*scalar), y(p1.y*scalar){};
    Point(Point p1, double s_x, double s_y): x(p1.x*s_x), y(p1.y*s_y){};
    inline double distance(Point &p){ return sqrt(pow(this->x-p.x, 2) + pow(this->y-p.y, 2)); }
    bool operator == (const Point &p) const{
        return this->x == p.x && this->y == p.y;
    }
    bool operator != (const Point &p) const{
        return this->x != p.x || this->y != p.y;
    }
    friend ostream& operator << (ostream &out, const Point& p) {
        out << fixed << setprecision(17) << p.x << "  " << p.y;
        return out;
    }
    friend istream& operator>>(istream &input, Point &p){
      input >> p.x >> p.y;
      return input;
    }
    const double& operator[](int i) const{
        if(i < 0 || i >=2)
            throw invalid_argument("point range out of bounds");
        return i == 0 ? this->x : this->y;
    }
};

class Color{
    public:
        uchar r, g, b;
        Color(): r(255), g(255), b(255){};
        Color(uchar _r, uchar _g, uchar _b): r(_r), g(_g), b(_b){};
        const bool operator==(const Color &c) const{
            return this->r == c.r && this->g == c.g && this->b == c.b;
        }

        const bool operator!=(const Color &c) const {
            return this->r != c.r || this->g != c.g || this->b != c.b;
        }
        
        friend ostream &operator<<(ostream &out, const Color &c){
            out << (int)c.r << " " << (int)c.g << " " << (int)c.b << " ";
            return out;
        }
};

typedef array<array<double, 2>, 2> Bounds;
const Bounds B0 = {{{0, 1}, {0, 1}}};

template<typename T>
class KDNode{
    private:
        KDNode *_l, *_r;
        T _value;
        int dim;
    public:
        Bounds bounds;

        KDNode(T val, int i): _l(NULL), _r(NULL), _value(val), dim(i) {};

        ~KDNode(){
            delete this->_l;
            delete this->_r;
        }
        KDNode<T>* get_left(){ return this->_l; }
        KDNode<T>* get_right(){ return this->_r; }
        T get_value(){ return this->_value; }
        int get_dim(){ return this->dim; };
        
        void set_left(KDNode<T> *n){ this->_l = n; }
        void set_right(KDNode<T> *n){ this->_r = n; }
        void set_value(T val){this->_value = val; }
        void set_dim(int i){ this->dim = i; }
    
        const bool leaf() const {
            return this->_l == NULL && this->_r == NULL;
        }
    
        static KDNode<T>* insert(KDNode<T> *root, T value, vector<KDNode<T>*> &nodelist, int d = 0, Bounds b = B0){
            int i = d % KD;
            if(root == NULL){
                KDNode<T> *tmp = new KDNode<T>(value, i);
                tmp->bounds = b;
                nodelist.push_back(tmp);
                return tmp;
            }

            Bounds lb, rb;

            lb[!i] = b[!i];
            rb[!i] = b[!i];
            lb[i] = {b[i][0], root->get_value()[i]};
            rb[i] = {root->get_value()[i], b[i][1]};

            double comp = root->get_value()[i];
            
            if(value[i] < comp){
                root->set_left(KDNode<T>::insert(root->get_left(), value, nodelist, d+1, lb));
            }else{
                root->set_right(KDNode<T>::insert(root->get_right(), value, nodelist, d+1, rb));
            }
            return root;
        }
    
        static void preorder(KDNode<T> *root){
            if(root == NULL)
                return;
            cout << root->get_value();
            preorder(root->get_left());
            preorder(root->get_right());
        }
};

int N = 0;
const Color WHITE(255,255,255);
const Color BLACK(0,0,0);
const Color RED(255, 0, 0);
const Color BLUE(0, 0, 255);

static inline void set_pixel(Color **c, double x, double y, Color color){
    if(x >= X || x < 0 || y >= Y || y < 0){
        return;
    }
    c[(int)x][(int)y] = color;
}

static inline void set_pixel(Color **c, Point p, Color color){
    set_pixel(c, p.x, p.y, color);
}

void draw_line(Color **c, Point p1, Point p2, Color colar){ // all cases
    int dx = p2.x - p1.x;
    int dy = p2.y - p1.y;
    
    int i1 = 0;
    int i2 = 0;
    int j1 = 0;
    int j2 = 0;
    i1 = dx > 0 ? 1 : -1;
    i2 = dx > 0 ? 1 : -1;
    j1 = dy > 0 ? 1 : -1;
    
    int i = std::abs(dx);
    int j = std::abs(dy);
    if(j>i){ // find DA
        std::swap(i,j);
        j2 = dy > 0 ? 1 : -1;
        i2 = 0;
    }
    
    int e = j/i;
    int x0 = p1.x; // keep the Point ints intact
    int y0 = p1.y;
    for(int k=0;k<=i;k++){
        set_pixel(c,x0,y0,colar);
        e += j;
        if(e>i){
            e -= i;
            x0 += i1;
            y0 += j1;
        }else{
            x0 += i2;
            y0 += j2;
        }
    }
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
        set_pixel (c, center.x+x, center.y+y, color);
        set_pixel (c, center.x+x, center.y-y, color);
        
        set_pixel (c, center.x-x, center.y+y, color);
        set_pixel (c, center.x-x, center.y-y, color);
        
        set_pixel (c, center.x+y, center.y+x, color);
        set_pixel (c, center.x+y, center.y-x, color);
        
        set_pixel (c, center.x-y, center.y+x, color);
        set_pixel (c, center.x-y, center.y-x, color);
        
        y2_new -= (2 * x) - 3;
    }
}

void write_ppm(Color **colors){
    ofstream out(OUTFILE);
    out << "P3" << endl << X << " " << Y << endl << 255 << endl;
    for(int i=0;i<X;i++)
        for(int j=0;j<Y;j++)
            out << colors[j][i];
    out.close();
}

void write_points(vector<Point> &points){
    remove(INFILE);
    ofstream out(INFILE);
    for(const Point &p: points)
        out << p << endl;
    out.close();
}

vector<Point>* input(){
    string in;
    do{
        cout << "Generate Points? yes/no ";
        cin >> in;
    }while(in != "yes" && in != "no");

    vector<Point> *points = new vector<Point>();
    if(in == "yes"){
        for(int i=0;i<10;i++)
            points->push_back(Point());
        write_points(*points);
        return points; 
    }else{
        ifstream fin(INFILE);
        Point p;
        while(fin >> p)
            points->push_back(p);
        fin.close();
        return points;
    }
}

void draw_diagram(Color **colors, vector<KDNode<Point>*> &nodelist){
    for(auto tree: nodelist){
        int d = tree->get_dim();
        Bounds b = tree->bounds;
        Point p = Point(tree->get_value(), X, Y);
        double lower = b[!d][0] * X;
        double upper = b[!d][1] * X;

        if(tree->get_dim() == 0){
            draw_circle(colors, p, 2.0, RED);
            draw_line(colors, Point(p.x, lower), Point(p.x, upper), RED);
        }else{
            draw_circle(colors, p, 2.0, BLUE);
            draw_line(colors, Point(lower, p.y), Point(upper, p.y), BLUE);
        }
    }
}

void part3(){
    vector<Point> *points = input();
    vector<KDNode<Point>*> nodelist;
    
    KDNode<Point> *tree = NULL;
    for(const Point &p: *points)
        tree = KDNode<Point>::insert(tree, p, nodelist);

    Color** colors = new Color*[X];
    for(int i=0;i<X;i++){
        colors[i] = new Color[Y];
    }
    draw_diagram(colors, nodelist);
    write_ppm(colors);
        
    for(int i=0;i<X;i++)
        delete[] colors[i];
    delete[] colors;
    delete tree;
    delete points;
    

}


int main(int argv, char* argc[]){
    srand(time(0));
    part3();
}