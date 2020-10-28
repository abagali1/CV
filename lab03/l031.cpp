// Anup Bagali
// 10/27/2020
// Period 7
#include <list>
#include <cmath>
#include <cfloat>
#include <iterator>
#include <iostream>

#define X 800
#define Y 800

using namespace std;

class Color{
    public:
        int r; int g; int b;
        Color(){
            this->r=0;
            this->g=0;
            this->b=0;
        };
        Color(int r, int g, int b){
            this->r=r;
            this->g=g;
            this->b=b;
        }
};

const Color BLACK(0, 0, 0);
const Color WHITE(255, 255, 255);
const Color RED(255, 0, 0);

class Point {
    public:
        double x,y;
    Point(){
        x = 0;y = 0;
    }
    Point(double px, double py){
        x = px; y = py;
    }
    Point(Point p1, double scalar){
        x = p1.x*scalar;
        y = p1.y*scalar;
    }
    void print(){
        printf("X: %e. Y: %e\n", this->x, this->y);
    }
};

class Line {
    public:
        double a, b, c;
    Line(double a0, double b0, double c0){
        a = a0; b = b0; c = c0;
    }
    Line(Point p1, Point p2){
        a = p2.y - p1.y;
        b = p1.x - p2.x;
        c = a*(p1.x) + b*(p1.y);
    }
    void print(){
        printf("A: %e B: %e, C: %e\n", this->a, this->b, this->c);
    }
};


static inline void set_pixel(Color** c, double x, double y, Color color){
    if(x >= X || x < 0 || y >= Y || y < 0){
        return;
    }
    c[(int)x][(int)y] = color;
}

static inline double distance(Point p1, Point p2){
    return sqrt( pow(p2.x-p1.x, 2) + pow(p2.y-p1.y,2));
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

list<Point> part1(list<Point> points){
    list<Point> min;
    double min_distance = DBL_MAX;
    for(list<Point>::iterator i = points.begin(); i != points.end(); ++i){
        for(list<Point>::iterator j = i; j != points.end(); ++j){
            if(i != j){
                double d = distance(*i, *j);
                if(d < min_distance){
                    min_distance = d;
                    min.clear();
                    min.push_back(*i);
                    min.push_back(*j);
                }
            }
        }
    }
    return min;
}

int main(){
    srand(time(0));
    FILE* fout;
    list<Point> points;
    Color** colors = new Color*[X];
    for(int i=0;i<X;i++){
        colors[i] = new Color[Y];
        for(int j=0;j<Y;j++){
            colors[i][j] = WHITE;
        }
    }
    fout = fopen("points.txt", "w");
    for(int i=0;i<50;i++){
        Point p(rand()/(double)RAND_MAX, rand()/(double)RAND_MAX);
        set_pixel(colors, p.x*X, p.y*Y, BLACK);
        draw_circle(colors, Point(p, X), 2.0, BLACK);
        points.push_back(p);
        fprintf(fout, "%0.25lf %0.25lf\n", p.x, p.y);
    }

    list<Point> min = part1(points);
    for(list<Point>::iterator i = min.begin(); i != min.end(); ++i){
        Point p = *i;
        set_pixel(colors, p.x*X, p.y*Y, RED);
        draw_circle(colors, Point(p, X), 2.0, RED);
    }

    fout = fopen("output.ppm", "w");
    fprintf(fout, "P3\n%d %d\n255\n", X, Y);
    for(int i=0;i<X;i++){
        for(int j=0;j<Y;j++){
            fprintf(fout, "%d %d %d\n", colors[i][j].r, colors[i][j].g, colors[i][j].b);
        }
    }
    fclose(fout);
    delete[] colors;
}