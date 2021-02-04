// Anup Bagali
// Period 7
// 12/16/2020
#include <bits/stdc++.h>

#define uchar unsigned char

#define X 800
#define Y 800
#define INFILE "points.txt"
#define OUTFILE "diagram.ppm"

using namespace std;

class Point {
    public:
        double x, y;
    Point(){
        this->x = rand()/(double)RAND_MAX;
        this->y = rand()/(double)RAND_MAX;
    }
    Point(double _x, double _y): x(_x), y(_y){};
    Point(Point p1, double scalar): x(p1.x*scalar), y(p1.y*scalar){};
    Point(Point p1, double s_x, double s_y): x(p1.x*s_x), y(p1.y*s_y){};
    inline double distance(Point &p){ return sqrt(pow(this->x-p.x, 2) + pow(this->y-p.y, 2)); }
    void print(){ printf("X: %e. Y: %e\n", this->x, this->y); };
    bool operator == (const Point &p) const{
        return this->x == p.x && this->y == p.y;
    }
    bool operator != (const Point &p) const{
        return this->x != p.x || this->y != p.y;
    }
};

class PointHash { 
    public: 
        size_t operator()(const Point &p) const
        { 
            return hash<double>()(p.x) ^ hash<double>()(p.y);
        } 
}; 

class Color{
    public:
        uchar r, g, b;
        Color(): r(255), g(255), b(255){};
        Color(uchar _r, uchar _g, uchar _b): r(_r), g(_g), b(_b){};
        void print(){ printf("R: %d, G: %d, B: %d\n", this->r, this->g, this->b); };
};


template<typename T>
class Node{
    public:
        Node* l;
        Node* r;
        T value;

        Node(T _v): l(NULL), r(NULL), value(_v){};

        static int height(Node<T>* n){
            if(n == NULL)
                return 0;
            int lh = Node<T>::height(n->l);
            int rh = Node<T>::height(n->r);

            return max(lh, rh) + 1;
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

void read_file(vector<Point> &points){
    points.clear();
    ifstream fin (INFILE);
    string line;
    remove(OUTFILE);
    while(getline(fin, line)){
        int s = line.find(" ");
        points.push_back(Point(stod(line.substr(0, s), NULL), stod(line.substr(s+1,s), NULL)));
        N++;
    }
}

void write_points(vector<Point> &points){
    FILE* fout = fopen(INFILE, "w");
    for(Point &p: points)
        fprintf(fout, "%0.17lf %0.17lf\n", p.x, p.y);
    fclose(fout);
    N = 10;
}

void write_ppm(Color **colors){
    FILE* fout = fopen(OUTFILE, "w");
    fprintf(fout, "P3\n%d %d\n255\n", X, Y);
    for(int i=0;i<X;i++){
        for(int j=0;j<Y;j++){
            fprintf(fout, "%d %d %d\n", colors[i][j].r, colors[i][j].g, colors[i][j].b);
        }
    }
    fclose(fout);
}

void draw_diagram(Color **c, Node<Point> *tree, int lb, int rb, int d = 0){
    if(tree == NULL)
        return;
    
    if(!d){
        draw_circle(c, tree->value, 3.0, RED);
        draw_diagram(c, tree->l, lb, tree->value.y, !d);
        draw_line(c, Point(lb, tree->value.y), Point(rb, tree->value.y), RED);
    }else{
        draw_circle(c, tree->value, 3.0, BLUE);
        draw_diagram(c, tree->l, lb, tree->value.x, !d);
        draw_line(c, Point(tree->value.x, lb), Point(tree->value.x, rb), BLUE);
    }
}

Node<Point>* insert(Point p, Node<Point> *tree = NULL, int d = 0){
    if(tree == NULL){
        tree = new Node<Point>(p);
    }else if(d == 0){
        if(p.x < tree->value.x)
            tree->l = insert(p, tree->l, !d);
        else
            tree->r = insert(p, tree->r, !d);
    }else{
        if(p.y > tree->value.y)
            tree->l = insert(p, tree->l, !d);
        else
            tree->r = insert(p, tree->r, !d);
    }
    return tree;
}

void part3(){
    string in;
    do{
        cout << "Generate Points? yes/no ";
        cin >> in;
    }while(in != "yes" && in != "no");
   
    vector<Point> points(10);
    if(in == "yes")
        write_points(points);
    else if(in == "no")
        read_file(points);
    
    Node<Point> *tree = insert(Point(points[0], X, Y));
    for(int i=1;i<N;i++)
        insert(Point(points[i], X ,Y), tree);

    Color** colors = new Color*[X];
    for(int i=0;i<X;i++){
        colors[i] = new Color[Y];
    }

    draw_diagram(colors, tree, 0, X, 0);
    write_ppm(colors);    

}


int main(int argv, char* argc[]){
    srand(time(0));
    part3();
}