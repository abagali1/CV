// Anup Bagali
// Period 7
// 12/16/2020
#include <bits/stdc++.h>

#define uchar unsigned char

#define X 800
#define Y 800
#define K 5
#define N 100
#define OUTFILE "cluster.ppm"


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
    inline double distance(Point& p){ return sqrt(pow(this->x-p.x, 2) + pow(this->y-p.y, 2)); }
    void print(){ printf("X: %e. Y: %e\n", this->x, this->y); };
    bool operator == (const Point& p) const{
        return this->x == p.x && this->y == p.y;
    }
    bool operator != (const Point& p) const{
        return this->x != p.x || this->y != p.y;
    }
};

class PointHash { 
    public: 
        size_t operator()(const Point& p) const
        { 
            return p.x + p.y;
        } 
}; 

class Color{
    public:
        uchar r, g, b;
        Color(): r(255), g(255), b(255){};
        Color(uchar _r, uchar _g, uchar _b): r(_r), g(_g), b(_b){};
        void print(){ printf("R: %d, G: %d, B: %d\n", this->r, this->g, this->b); };
};

const Color WHITE(255,255,255);
const Color BLACK(0,0,0);

static inline void set_pixel(Color** c, double x, double y, Color color){
    if(x >= X || x < 0 || y >= Y || y < 0){
        return;
    }
    c[(int)x][(int)y] = color;
}

static inline void set_pixel(Color** c, Point p, Color color){
    set_pixel(c, p.x, p.y, color);
}

void draw_circle(Color** c, Point center, double r, Color color){
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

void write_file(Color** colors){
    FILE* fout = fopen(OUTFILE, "w");
    fprintf(fout, "P3\n%d %d\n255\n", X, Y);
    for(int i=0;i<X;i++){
        for(int j=0;j<Y;j++){
            fprintf(fout, "%d %d %d\n", colors[i][j].r, colors[i][j].g, colors[i][j].b);
        }
    }
    fclose(fout);
}

void gen_palette(Color palette[K]){
    palette[0] = Color(0,0,0);
    palette[1] = Color(255, 0, 0);
    palette[2] = Color(0, 255, 0);
    palette[3] = Color(0, 0, 255);
    palette[4] = Color(255, 255, 0);
    /*    
    for(int i=1;i<K;i++){
        int d = max(K*(int)((i-1)/3), 1);
        switch ((i-1)%3){
        case 0:
            palette[i] = Color((int)255/d, 0, 0);
            break;
        case 1:
            palette[i] = Color(0, (int)255/d, 0);
            break;
        case 2:
            palette[i] = Color(0, 0, (int)255/d);
            break;
        default:
            break;
        }
    }*/
}

Point mean_for_point(Point means[K], Point p){
    Point m = means[0];
    double md = p.distance(means[0]);
    for(int i=1;i<K;i++){
        double d = p.distance(means[i]);
        if(md > d){
            md = d;
            m = means[i];
        }
    }
    return m;
}

bool reorganize(unordered_map<Point, vector<Point>, PointHash>* previous, Point means[K], vector<Point>* points){
    Point new_means[K];
    for(int i=0;i<K;i++){
        vector<Point> points = previous->at(means[i]); 
        int n = points.size();
        double x = 0;
        double y = 0;
        for(int j=0;j<n;j++){
            x += points[j].x;
            y += points[j].y;
        }
        new_means[i] = Point(x/n, y/n);
    }
    previous->clear();
    for(int i=0;i<N;i++){
        Point p = points->at(i);
        (*previous)[mean_for_point(new_means, p)].push_back(p); 
    }
    bool organized = true;
    for(int i=0;i<K;i++){
        if(means[i] != new_means[i])
            organized = false;
        means[i] = new_means[i];
    }
    return organized;
}

void part1(){
    Point means[K];
    Color palette[K];
    Color** colors = new Color*[X];
    for(int i=0;i<X;i++){
        colors[i] = new Color[Y];
    }
    vector<Point>* points = new vector<Point>(N);
    gen_palette(palette);

    unordered_map<Point, vector<Point>, PointHash> organized_means;
    for(int i=0;i<N;i++){
        organized_means[mean_for_point(means, points->at(i))].push_back(points->at(i));
    }
    
    bool organized = false;
    do{
        organized = reorganize(&organized_means, means, points);
        for(int i = 0; i<K;i++){
            means[i].print();
        }
        cout << organized << endl;
    }while(!organized);


    write_file(colors);

    delete[] colors;
    delete points;
}


int main(int argv, char* argc[]){
    srand(time(0));
    part1();
}