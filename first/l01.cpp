// Anup Bagali
// Sept. 8, 2020
// Period 7
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <algorithm>

#define X 800
#define Y 800


class Point {
    public:
        double x,y;
    Point(){
        x = 0;
        y = 0;
    }
    Point(double px, double py){
        x = px; 
        y = py;
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
        a = p2.y - p1.y;
        b = p1.x - p2.x;
        c = a*(p1.x) + b*(p1.y);
    }
    void print(){
        printf("A: %e B: %e, C: %e\n", this->a, this->b, this->c);
    }
};

static inline void set_pixel(int** c, double x, double y){
    if(x > X || x < 0 || y > Y || y < 0){
        return;
    }
    c[(int)x][(int)y] = 0;
}

static inline double distance(Point p1, Point p2){
    return sqrt( pow(p2.x-p1.x, 2) + pow(p2.y-p1.y,2));
}

double area(Point points[3]){
    return points[0].x * (points[1].y-points[2].y) + points[1].x * (points[2].y-points[0].y) + points[2].x * (points[0].y - points[1].y);
}


void draw_line(int** c, Point p1, Point p2){ // all cases
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
        set_pixel(c,x0,y0);
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

void draw_circle(int** c, Point center, double r){
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
        set_pixel (c, center.x+x, center.y+y);
        set_pixel (c, center.x+x, center.y-y);
        
        set_pixel (c, center.x-x, center.y+y);
        set_pixel (c, center.x-x, center.y-y);
        
        set_pixel (c, center.x+y, center.y+x);
        set_pixel (c, center.x+y, center.y-x);
        
        set_pixel (c, center.x-y, center.y+x);
        set_pixel (c, center.x-y, center.y-x);
        
        y2_new -= (2 * x) - 3;
    }
}

double* find_radii(Point vertices[3], double* s, double* r, double* R){
    double sum = 0.0;
    static double ds[3];
    double d;
    for(int i=0;i<3;i++){
        d = distance(vertices[i], vertices[(i+1)%3]);
        sum += d;
        ds[i] = d;
    }
    *s = sum*0.5;
    *r = sqrt((((*s)-ds[0])*((*s)-ds[1])*((*s)-ds[2]))/(*s));
    *R = (ds[0]*ds[1]*ds[2])/(4*(*r)*(*s));
    return ds;
}

Point intersection(Line l1, Line l2){
    double det = l1.a*l2.b - l2.a*l1.b;
    return Point((l2.b*l1.c - l1.b*l2.c)/det, (l1.a*l2.c - l2.a*l1.c)/det);
}

Line perpendicular_bisector(Line l, Point p1, Point p2){
    Point mid = Point((p1.x+p2.x)/2, (p1.y+p2.y)/2);
    return Line(-l.b,l.a,-l.b*(mid.x)+l.a*(mid.y));
}

Point find_circumcenter(Point verts[3]){
    for(int i=0;i<3;i++){
        printf("%e %e\n",verts[i].x, verts[i].y);
    }
    Line l1 = Line(verts[0], verts[1]);
    Line l2 = Line(verts[1], verts[2]);
    
    Line p1 = perpendicular_bisector(l1, verts[0], verts[1]);
    Line p2 = perpendicular_bisector(l2, verts[1], verts[2]);
    return intersection(p1,p2);
}

int main(int argc, char** argv){
    FILE* fout;
    int** colors = new int*[X];
    for(int x=0;x<X;x++){
        colors[x] = new int[Y];
        for(int y=0;y<Y;y++){
            colors[x][y] = 1;
        }
    }
    Point vertices[3];
    do {
        for(int i = 0;i<3;i++){
            vertices[i] = Point((int)rand() % X, (int)rand() % Y);
        }
    }while(area(vertices) == 0.0);
    
    for(int i=0;i<3;i++){
        Point p1 = vertices[i];
        Point p2 = vertices[(i+1)%3];
        draw_line(colors, p1, p2);
    }
    
    double s, inr, outr;
    double* dists = find_radii(vertices, &s, &inr, &outr);
    
    Point incenter = Point(
        (int)( ( (dists[1] * vertices[0].x)+(dists[2]*vertices[1].x)+(dists[0]*vertices[2].x ) ) /(s*2.0)),
        (int)(((dists[1] * vertices[0].y)+(dists[2]*vertices[1].y)+(dists[0]*vertices[2].y))/(s*2.0))
    );
    Point circumcenter = find_circumcenter(vertices);
    draw_circle(colors, incenter, inr);
    draw_circle(colors, circumcenter, outr);
    
    
    

    fout = fopen("triangles.ppm", "w");
    fprintf(fout, "P3\n%d %d\n1\n", X, Y);
    for (int Py = 0; Py < Y; Py++)
    {
        for (int Px = 0; Px < X; Px++)
        {
            fprintf(fout, "%d %d %d\n",
            colors[Px][Py],colors[Px][Py], colors[Px][Py]);
        }
    }
    fclose(fout);
    return 0;
}

