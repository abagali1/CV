// Anup Bagali
// Period 7
// 03/08/2021

#include <bits/stdc++.h>

#define uchar unsigned char

#define INFILE "image.ppm"
#define GOUTFILE "imageg.ppm"
#define MOUTFILE "imagem.ppm"

#define T 500

using namespace std;

class Color{
    public:
        uchar r, g, b;
        Color(): r(255), g(255), b(255){};
        Color(uchar _r, uchar _g, uchar _b): r(_r), g(_g), b(_b){};
        inline int gray(){ return (this->r+this->b+this->g)/3; }
        inline double distance(Color& c){ return sqrt(pow(this->r-c.r, 2)) + sqrt(pow(this->g-c.g,2)) + sqrt(pow(this->b-c.b, 2)); }
        inline void print(){ printf("R: %d, G: %d, B: %d\n", this->r, this->g, this->b); };
        
        bool operator==(const Color& c) const{
            return this->r == c.r && this->g == c.g && this->b == c.b;
        }
    
        bool operator!=(const Color& c) const{
            return this->r != c.r || this->g != c.g || this->b != c.b;
        }
};


int X, Y, N;
const Color WHITE(255,255,255);
const Color BLACK(0,0,0);

const vector<int> SX{1, 0, -1, 2, 0, -2, 1, 0, -1};
const vector<int> SY{1, 2, 1, 0, 0, 0, -1, -2 ,-1};

int** read_file(){
    string line;
    ifstream fin(INFILE);
    remove(GOUTFILE);
    remove(MOUTFILE);
    
    getline(fin, line);
    getline(fin, line);
    
    int s = line.find(" ");
    X = stoi(line.substr(0, s));
    Y = stoi(line.substr(s+1));
    N = X*Y;

    int **colors = new int*[X];
    for(int i=0;i<X;i++)
        colors[i] = new int[Y];

    getline(fin, line);
        
    int i = 0;
    int j = 0;
    FILE* fout = fopen(GOUTFILE, "w");
    fprintf(fout, "P3\n%d %d\n255\n", X, Y);
    while(getline(fin, line)){
        istringstream is(line);
        string part;
        uchar t[3];
        while(getline(is, part, ' ')){
            t[j] = stoi(part);
            j++;
            if(j == 3){
                Color tmp = Color(t[0], t[1], t[2]);
                colors[i/Y][i%Y] = tmp.gray();
                fprintf(fout, "%d %d %d ", tmp.gray(), tmp.gray(), tmp.gray());
                j = 0;
                i++;
            }
        }
        fprintf(fout, "\n");
    }
    fclose(fout);
    return colors;
}

void write_result(Color **colors){
    FILE* fout = fopen(MOUTFILE, "w");
    fprintf(fout, "P3\n%d %d\n255\n", X, Y);
    for(int i=0;i<X;i++)
        for(int j=0;j<Y;j++)
            fprintf(fout, "%d %d %d\n", colors[i][j].r, colors[i][j].g, colors[i][j].b);
    fclose(fout);
}

double dot(vector<int> &p, vector<int> &op){
    return 1.0;
}


void sobel_threshold(int **grayscale){

}

void part1(){
    int **g = read_file();
    // sobel_threshold(g);

    for(int i=0;i<X;i++)
        delete[] g[i];
    delete[] g;
}


int main(int argf, char* argc[]){
    part1();
}
