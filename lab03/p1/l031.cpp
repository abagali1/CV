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
#define uchar unsigned char

using namespace std;

class Color{
    public:
        uchar r, g, b;
        Color(){
            this->r=0;
            this->g=0;
            this->b=0;
        };
        Color(uchar r, uchar g, uchar b){
            this->r=r;
            this->g=g;
            this->b=b;
        }
        void print(){
            printf("R: %d, G: %d, B: %d\n", this->r, this->g, this->b);
        }
};

const Color BLACK(0, 0, 0);
const Color WHITE(255, 255, 255);
const Color RED(255, 0, 0);

class Point {
    public:
        double x, y;
    Point(){
        this->x = 0;
        this->y = 0;
    }
    Point(double x, double y){
        this->x = x;
        this->y = y;
    }
    Point(Point p1, double scalar){
        this->x = p1.x*scalar;
        this->y = p1.y*scalar;
    }
    Point(Point p1, double s_x, double s_y){
        this->x = p1.x*s_x;
        this->y = p1.y*s_y;
    }
    void print(){
        printf("X: %e. Y: %e\n", this->x, this->y);
    }
};

static inline void set_pixel(Color** c, double x, double y, Color color){
    if(x >= X || x < 0 || y >= Y || y < 0){
        return;
    }
    c[(int)x][(int)y] = color;
}

static inline void set_pixel(Color** c, Point p, Color color){
    set_pixel(c, p.x, p.y, color);
}

static inline double distance(Point p1, Point p2){
    return sqrt(pow(p2.x-p1.x, 2) + pow(p2.y-p1.y,2));
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

void closest_point_n2(list<Point> points, Point* p1, Point* p2){
    double min_distance = DBL_MAX;
    for(list<Point>::iterator i = points.begin(); i != points.end(); ++i){
        for(list<Point>::iterator j = i; j != points.end(); ++j){
            if(i != j){
                double d = distance(*i, *j);
                if(d < min_distance){
                    min_distance = d;
                    *p1 = *i;
                    *p2 = *j;
                }
            }
        }
    }
}

void part1(){
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
    fclose(fout);

    Point p1;
    Point p2;
    closest_point_n2(points, &p1, &p2);
    
    p1 = Point(p1, X);
    p2 = Point(p2, X);
    set_pixel(colors, p1, RED);
    draw_circle(colors, p1, 2.0, RED);

    set_pixel(colors, p2, RED);
    draw_circle(colors, p2, 2.0, RED);
    

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

int main(){
    part1();
    return 0;
}
