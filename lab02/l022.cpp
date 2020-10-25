// Anup Bagali
// Oct. 10, 2020
// Period 7
#include <math.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <fstream>

#define X 800
#define Y 800
#define e 0.0001

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
};

double area(Point p1, Point p2, Point p3) 
{ 
   return abs((p1.x*(p2.y-p3.y) + p2.x*(p3.y-p1.y)+ p3.x*(p1.y-p2.y))/2.0); 
} 


bool in_triangle(Point verts[4]){
    double A = area(verts[0], verts[1], verts[2]);
    double s = area(verts[0], verts[1], verts[3]) + area(verts[0], verts[3], verts[2]) + area(verts[3], verts[1], verts[2]);
    return A-e <= s && A+e >= s;
}

double area(int x1, int y1, int x2, int y2, int x3, int y3) 
{ 
   return abs((x1*(y2-y3) + x2*(y3-y1)+ x3*(y1-y2))/2.0); 
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

void parse_file(Point* points){
    ifstream fin("points.txt");
    string line; 
    getline(fin, line);
    points[0] = Point(stod(line.substr(1,20), NULL), stod(line.substr(21,40),NULL));
    points[1] = Point(stod(line.substr(45,64), NULL), stod(line.substr(65,84), NULL));
    points[2] = Point(stod(line.substr(89,108), NULL), stod(line.substr(109,128), NULL));
    points[3] = Point(stod(line.substr(133, 152), NULL), stod(line.substr(153, 172), NULL));
}

void part2(){
    int** colors = new int*[X];
    for(int x=0;x<X;x++){
        colors[x] = new int[Y];
        for(int y=0;y<Y;y++){
            colors[x][y] = 1;
        }
    }
    Point points[4];
    parse_file(points);



}

int main(int argc, char* argv[]){
    srand(time(0));
    // part1();
    part2();
    

    return 0;
}

