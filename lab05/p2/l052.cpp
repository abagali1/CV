// Anup Bagali
// Period 7
// 03/18/2021

#include <bits/stdc++.h>

#define uchar unsigned char

#define INFILE "image.ppm"
#define OUT1 "image1.ppm"
#define OUT2 "image2.ppm"
#define OUTFILE "imagef.ppm"

#define T 300

using namespace std;

int X, Y, N;

const int SX[9] = {1, 0, -1, 2, 0, -2, 1, 0, -1};
const int SY[9] = {1, 2, 1, 0, 0, 0, -1, -2 ,-1};

static inline bool edge(int i){
    return (i < X) || (!(i%X)) || !((i+1)%X) || (i > X*(Y-1));
}

static inline int dot(const int p[9], const int op[9]){
    return p[0]*op[0] + p[1]*op[1] + p[2]*op[2] + p[3]*op[3] + p[4]*op[4] + p[5]*op[5] + p[6]*op[6] + p[7]*op[7] + p[8]*op[8];
}

void write_ppm(const string &filename, vector<int> &img, int size = 1){
    ofstream out(filename);
    out << "P3" << endl << X << " " << Y << endl << size << endl;
    for(int d: img)
        out << d << " " << d << " " << d << " ";
    out.close();
}

void write_ppm(const string &filename, vector<vector<int>> &img, int size = 1){
    ofstream out(filename);
    out << "P3" << endl << X << " " << Y << endl << size << endl;
    for(int i=0;i<X;i++)
        for(int j=0;j<Y;j++)
            out << img[j][i] << " " << img[j][i] << " " << img[j][i] << " ";
    out.close();
}

vector<int>* read_file(const string &filename){
    ifstream fin(filename);
    string format;
    int size; 
    fin >> format >> X >> Y >> size;
    N = X*Y;

    int p1, p2, p3;
    vector<int> *grayscale = new vector<int>();
    for(int i=0;i<N;i++){
        fin >> p1 >> p2 >> p3;
        grayscale->push_back((p1+p2+p3)/3);
    }
    fin.close();
    return grayscale;
}

void single_threshole(vector<int> &grayscale){
    vector<int> *gradient = new vector<int>(N, 0);
    int gx, gy;
    for(int i=X+1;i<N-X;i++){
        if(edge(i)){
            gradient->at(i) = 0;
        }else{
            int grad[9] = {
                grayscale[i-X-1], grayscale[i-X], grayscale[i-X+1],
                grayscale[i-1]  , grayscale[i]  , grayscale[i+1]  ,
                grayscale[i+X-1], grayscale[i+X], grayscale[i+X+1],
            };
            gx = dot(grad, SX);
            gy = dot(grad, SY);
            gradient->at(i) = (int)(sqrt(pow(gx, 2) + pow(gy, 2)) > T);
        }
    }
    write_ppm("test.ppm", *gradient);
    delete gradient;
}

void part2(){
    vector<int> *grayscale = read_file("image.ppm");
    single_threshole(*grayscale);
    delete grayscale;
}

int main(int argc, char *argv[]){
    part2();
}

