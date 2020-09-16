// Anup Bagali
// Sept. 8, 2020
// Period 7
#include <cmath>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <algorithm>

#define X 800
#define Y 800


typedef struct {
    int x;
    int y;
} Point;

void set_pixel(int c[][Y], int x, int y){
    std::cout << "s" << std::endl;
    c[x][y] = 1;
}


void line(int c[][Y], Point p1, Point p2){
    int dx = p1.x - p2.x;
    int dy = p1.y - p2.y;
    int e = dy-dx;
    int j = p1.y;
    for(int i=p1.x;i<p2.x;i++){
        set_pixel(c, i,j);
        if(e>=0){
            j++;
            e -= dx;
        }
        e += dy;
    }

}


int main(int argc, char** argv){
    FILE* fout;
    int colors[X][Y];

    srand(1738115);
  
    line(colors,(Point){.x=800,.y=800}, (Point){.x=0,.y=0});
  /*  
    Point vertices[3] = {
        (Point){.x = 100, .y = 100},
        (Point){.x = 200, .y = 200},
        (Point){.x = 300 % X, .y = 300}
    };

    for(int v=0;v<2;v++){
        Point p1 = vertices[v];
        Point p2 = vertices[(v+1)%3];

        std::cout << p1.x << " " << p1.y << std::endl;
        std::cout << p2.x << " " << p2.y << std::endl;
        line(colors,p1,p2);    
    }
*/



    fout = fopen("triangles.ppm", "w");
    fprintf(fout, "P3\n");
    fprintf(fout, "%d %d\n", X, Y);
    fprintf(fout, "1\n");
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


