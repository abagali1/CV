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
    c[x][y] = 0;
}

int draw(int c[][Y], int i, int* j, int e, int dx, int dy, int ddy){
    int error = e;
    set_pixel(c, i, *j);
    if (e >= 0){
        *j += ddy/dy;
        error -= dx;
    }
    return error + dy;
}

void x_line(int c[][Y], Point p1, Point p2){
    int dx = std::abs(p2.x - p1.x);
    int ddy = p2.y - p1.y;
    int dy = std::abs(ddy);
    int e = dy-dx;
    int j = p1.y;

    if(p2.x >= p1.x) {
        for(int i=p1.x;i<p2.x;i++){
            e = draw(c, i, &j, e, dx, dy, ddy);
        }
    }else{
        for(int i=p1.x;i>p2.x;i--){
            e = draw(c, i, &j, e, dx, dy, ddy);
        }
    }

}


int main(int argc, char** argv){
    FILE* fout;
    int colors[X][Y];
    for(int x=0;x<X;x++){
        for(int y=0;y<Y;y++){
            colors[x][y] = 1;
        }
    }
  
    x_line(colors, (Point){.x=800,.y=800}, (Point){.x=0,.y=0});

/*    for(int i=0;i<X;i+=10){
        x_line(colors,(Point){.x=800,.y=i},(Point){.x=0,.y=0});
        x_line(colors,(Point){.x=800,.y=i},(Point){.x=0,.y=800});
    }
  
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


