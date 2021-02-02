// Anup Bagali
// Period 7
// 12/16/2020
#include <bits/stdc++.h>

#define uchar unsigned char

#define X 800
#define Y 800
#define K 5
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

void read_file(vector<Point>& points){
    points.clear();
    ifstream fin (INFILE);
    string line;
    remove(OUTFILE);
    while(getline(fin, line)){
        int s = line.find(" ");
        points.push_back(Point(stod(line.substr(0, s), NULL), stod(line.substr(s+1,s), NULL)));
    }
}

void write_points(vector<Point>& points){
    FILE* fout = fopen(INFILE, "w");
    for(int i=0;i<N;i++)
        fprintf(fout, "%0.17lf %0.17lf\n", points[i].x, points[i].y);
    fclose(fout);
}

void write_ppm(Color** colors){
    FILE* fout = fopen(OUTFILE, "w");
    fprintf(fout, "P3\n%d %d\n255\n", X, Y);
    for(int i=0;i<X;i++){
        for(int j=0;j<Y;j++){
            fprintf(fout, "%d %d %d\n", colors[i][j].r, colors[i][j].g, colors[i][j].b);
        }
    }
    fclose(fout);
}


void part3(){
    char i;
    cout << "Generate Points? y/n ";
    cin >> i;
   
    vector<Point> points(10);
    if(i == 'y')
        write_points(points);
    else
        read_file(points);
}


int main(int argv, char* argc[]){
    srand(time(0));
    part3();
}