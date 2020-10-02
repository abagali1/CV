// Anup Bagali
// Sept. 8, 2020
// Period 7
#include <stdio.h>
#include <stdlib.h>
#include <algorithm>


#define X 800
#define Y 800


class Point {
    public:
        int x,y;
    Point(int px, int py){
        x = px; 
        y = py;
    }
};

static inline void set_pixel(int** c, int x, int y){
    c[x][y] = 0;
}


int draw_x(int** c, int i, int* j, int e, int dx, int dy, int ddy){ // x case helper
    int error = e;
    set_pixel(c, i, *j);
    if (e >= 0){
        *j += ddy/dy;
        error -= dx;
    }
    return error + dy;
}

void x_line(int** c, Point p1, Point p2){ // x case old
    int dx = std::abs(p2.x - p1.x);
    int ddy = p2.y - p1.y;
    int dy = std::abs(ddy);
    int e = dy-dx;
    int j = p1.y;

    if(p2.x >= p1.x) {
        for(int i=p1.x;i<p2.x;i++){
            e = draw_x(c, i, &j, e, dx, dy, ddy);
        }
    }else{
        for(int i=p1.x;i>p2.x;i--){
            e = draw_x(c, i, &j, e, dx, dy, ddy);
        }
    }
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


int main(int argc, char** argv){
    FILE* fout;
    int** colors = new int*[X];
    for(int x=0;x<X;x++){
        colors[x] = new int[Y];
        for(int y=0;y<Y;y++){
            colors[x][y] = 1;
        }
    }
    
    Point vertices[3] = {
        Point(rand() % X, rand() % Y),
        Point(rand() % X, rand() % Y),
        Point(rand() % X, rand() % Y)
    };

    for(int i=0;i<3;i++){
        Point p1 = vertices[i];
        Point p2 = vertices[(i+1)%3];
        draw_line(colors, p1, p2);
    }

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

