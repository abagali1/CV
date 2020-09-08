// Anup Bagali
// Sept. 8, 2020
// Period 7
#include <stdio.h>
#include <stdlib.h>

#define X 800
#define Y 800

using namespace std;

typedef struct {
    int r;
    int g;
    int b;
} Color;

const Color BACKGROUND = (Color){.r=0, .g=0, .b=0};


int main(int argc, char** argv){
    FILE* fout;
    Color **colors = (Color**)malloc(sizeof(Color*) * Y);
    for(int i=0;i<Y;i++){
       colors[i] = (Color*)malloc(sizeof(Color) * X);
    }

    fout = fopen("triangles.ppm", "w");
    fprintf(fout, "P3\n");
    fprintf(fout, "%d %d\n", X, Y);
    fprintf(fout, "255\n");
    for (int Py = 0; Py < Y; Py++)
    {
        for (int Px = 0; Px < X; Px++)
        {
            fprintf(fout, "%d %d %d\n",
            colors[Py][Px].r, colors[Py][Px].g, colors[Py][Px].b);
        }
    }
    fclose(fout);
    return 0;
}


