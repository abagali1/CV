// Anup Bagali
// Oct. 25, 2020
// Period 7
#include <math.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <iostream>
#include <fstream>
#include <cfloat>

#define X 800
#define Y 800
#define ERR 0.0001

using namespace std;

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

static inline void set_pixel(int** c, double x, double y){
    if(x >= X || x < 0 || y >= Y || y < 0){
        return;
    }
    c[(int)x][(int)y] = 0;
}

static inline double distance(Point p1, Point p2){
    return sqrt( pow(p2.x-p1.x, 2) + pow(p2.y-p1.y,2));
}

static inline Point midpoint(Point p1, Point p2){
    return Point((p1.x+p2.x)/2, (p1.y+p2.y)/2);
}

static inline double slope(Point p1, Point p2){
    return (p2.y-p1.y)/(p2.x-p1.x);
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
        swap(i,j);
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

Point intersection(Line l1, Line l2){
    double det = l1.a*l2.b - l2.a*l1.b;
    return Point((l2.b*l1.c - l1.b*l2.c)/det, (l1.a*l2.c - l2.a*l1.c)/det);
}

Line perpendicular_bisector(Line l, Point p1, Point p2){
    Point mid = midpoint(p1,p2);
    return Line(-l.b,l.a,-l.b*(mid.x)+l.a*(mid.y));
}

double area(Point p1, Point p2, Point p3) 
{ 
   return abs((p1.x*(p2.y-p3.y) + p2.x*(p3.y-p1.y)+ p3.x*(p1.y-p2.y))/2.0); 
} 

bool in_triangle(Point verts[4]){
    double A = area(verts[0], verts[1], verts[2]);
    double s = area(verts[0], verts[1], verts[3]) + area(verts[0], verts[3], verts[2]) + area(verts[3], verts[1], verts[2]);
    return A-ERR <= s && A+ERR >= s;
}

void parse_file(Point* points){
    ifstream fin("points.txt");
    string line, buf;
    getline(fin, line);
    double temp[8];
    int i = 0;
    while(i < 8){
        for(const char &c: line){
            if(c == '.' || isdigit(c)){
                buf += c;
            }
            if(buf.length() == 19){
                temp[i] = stod(buf, NULL);
                buf = "";
                i++;
            }
        }
    }
    for(int j=0;j<4;j++){
        points[j] = Point(temp[j*2], temp[j*2+1]);
    }
}

void find_square(Point p1, Point p2, Point p3, Point p4, Point* output){
    double d = distance(p1, p2);
    double m = -1 / slope(p1, p2);

    Point p(p3.x + d * sqrt(1 / (1 + pow(m, 2))), p3.y + m * d * sqrt(1 / (1 + pow(m,2))));

    double ad = slope(p, p4);
    double bc = -1 / ad;

    Line l1(-bc, 1, -1*bc*p1.x+p1.y);
    Line l2(-bc, 1, -1*bc*p2.x+p2.y);
    Line l3(-ad, 1, -1*ad*p3.x+p3.y);
    Line l4(-ad, 1, -1*ad*p4.x+p4.y);
    
    output[0] = intersection(l1, l3);
    output[1] = intersection(l1, l4);
    output[2] = intersection(l2, l4);
    output[3] = intersection(l2, l3);
 }

int min_area(Point squares[6][4], double areas[6]){
    double min_area = DBL_MAX;
    int min = 0;
    for(int i=0;i<6;i++){
        areas[i] = pow(distance(squares[i][0], squares[i][1]), 2);
        if(areas[i] < min_area){
            min = i;
            min_area = areas[i];
        }
    }
    return min;
}

void part1(){
    Point vertices[4];
    for(int i=0;i<3;i++){
        vertices[i] = Point(rand()/(double)RAND_MAX, rand()/(double)RAND_MAX);
    }
    
    do{
        vertices[3] = Point(rand()/(double)RAND_MAX, rand()/(double)RAND_MAX);
    }while(in_triangle(vertices) == true);
    
    FILE* fout = fopen("points.txt","w");
    for(int i=0;i<3;i++){
        fprintf(fout, "(%0.17lf, %0.17lf) , ", vertices[i].x, vertices[i].y);
    }
    fprintf(fout, "(%0.17lf, %0.17lf)", vertices[3].x, vertices[3].y);
    fclose(fout);
}

void part2(){
    Point points[4];
    FILE* fout;
    parse_file(points);
    Point squares[6][4];
    double areas[6];
    find_square(points[0], points[1], points[2], points[3], squares[0]);
    find_square(points[0], points[2], points[1], points[3], squares[1]);
    find_square(points[0], points[3], points[1], points[2], squares[2]);
    find_square(points[0], points[1], points[3], points[2], squares[3]);
    find_square(points[0], points[3], points[2], points[1], squares[4]);
    find_square(points[0], points[2], points[3], points[1], squares[5]);

    int min_index = min_area(squares, areas);

    fout = fopen("output.txt", "w");
    for(int i=0;i<3;i++){
        fprintf(fout, "(%0.17lf, %0.17lf) , ", points[i].x, points[i].y);
    }
    fprintf(fout, "(%0.17lf, %0.17lf)\n", points[3].x, points[3].y);
    for(int i=0;i<6;i++){
        for(int j=0;j<3;j++){
            fprintf(fout, "(%0.17f, %0.17f) , ", squares[i][j].x, squares[i][j].y);
        }
        fprintf(fout, "(%0.17lf, %0.17lf) Area = %0.17f\n", squares[i][3].x, squares[i][3].y, areas[i]);
    }
    fclose(fout);

    int** colors = new int*[X];
    for(int x=0;x<X;x++){
        colors[x] = new int[Y];
        for(int y=0;y<Y;y++){
            colors[x][y] = 1;
        }
    }

    for(int i=0;i<4;i++){
        set_pixel(colors, squares[min_index][i].x*X, squares[min_index][i].y*Y);
        draw_circle(colors, Point(squares[min_index][i], 800.0), 4);
        draw_circle(colors, Point(points[i], 800.0), 2);
        Line l(Point(squares[min_index][i], 800.0), Point(squares[min_index][(i+1)%4],800.0));
        draw_line(colors, Point(0, l.c/l.b), Point(800, (l.c-800*l.a)/l.b));
    }

    fout = fopen("output.ppm", "w");
    fprintf(fout, "P3\n%d %d\n1\n", X, Y);
    for (int Py = 0; Py < Y; Py++){
        for (int Px = 0; Px < X; Px++){
            fprintf(fout, "%d %d %d\n",
            colors[Px][Py],colors[Px][Py], colors[Px][Py]);
        }
    }
    fclose(fout);
    delete [] colors;

}

int main(int argc, char* argv[]){
    srand(time(0));
    // part1();
    part2();
    

    return 0;
}

