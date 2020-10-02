// Anup Bagali
// Oct. 10, 2020
// Period 7
#include <math.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>

#define X 1
#define Y 1
#define e 0.0001


class Point {
    public:
        int x,y;
    Point(){
        x = 0;
        y = 0;
    }
    Point(int px, int py){
        x = px; 
        y = py;
    }
};

double area(Point p1, Point p2, Point p3){
    return std::abs((p1.x*(p2.y-p3.y) + p2.x*(p3.y-p1.y) + p3.x*(p1.y-p2.y))/2.0);
}

bool in_triangle(Point verts[4]){
    double A = area(verts[0], verts[1], verts[2]);
    double s = area(verts[0], verts[1], verts[3]) + area(verts[0], verts[3], verts[2]) + area(verts[3], verts[1], verts[2]);
    
    return A-e <= s && A+e >= s;
}

float area(int x1, int y1, int x2, int y2, int x3, int y3) 
{ 
   return abs((x1*(y2-y3) + x2*(y3-y1)+ x3*(y1-y2))/2.0); 
} 

int main(int argc, char** argv){
    FILE* fout;
    srand(time(0));
    Point vertices[4];
    for(int i=0;i<3;i++){
        vertices[i] = Point(rand() % X, rand() % Y);
    }
    
    do{
        vertices[3] = Point(rand() % X, rand() % Y);
    }while(in_triangle(vertices) == true);
    
    fout = fopen("points.txt", "w");
    for(int i =0;i<4;i++){
        fprintf(fout, " (%d %d) ,", vertices[i].x, vertices[i].y);
    }
    fclose(fout);
    return 0;
}
