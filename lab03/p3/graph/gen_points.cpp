#include <bits/stdc++.h>

int main(int argv, char* argc[]){
    int N = atoi(argc[1]);
    FILE* fout = fopen("points.txt", "w");
    for(int i=0;i<N;i++){
        double x = rand()/(double)RAND_MAX;
        double y = rand()/(double)RAND_MAX;
        fprintf(fout, "%0.25lf %0.25lf\n", x, y);
    }
    fclose(fout);
}
