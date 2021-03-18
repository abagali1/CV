// Anup Bagali
// Period 7
// 03/18/2021

#include <bits/stdc++.h>

#define uchar unsigned char

#define INFILE "image.ppm"
#define OUT1 "image1.ppm"
#define OUT2 "image2.ppm"
#define OUTFILE "imagef.ppm"

using namespace std;

int X, Y, N;

const int SX[9] = {1, 0, -1, 2, 0, -2, 1, 0, -1};
const int SY[9] = {1, 2, 1, 0, 0, 0, -1, -2 ,-1};

template<typename C, int N>
void print_array(C arr[N]){
    for(int i=0;i<N;i++)
        cout << arr[i] << ", ";
    cout << endl;
}

void write_ppm(const string &filename, vector<double> &img){
    ofstream out(filename);
    out << "P3" << endl << X << " " << Y << endl << "1" << endl;
    for(double d: img)
        out << (int)round(d) << " " << (int)round(d) << " " << (int)round(d) << " ";
    out.close();
}

void write_ppm(const string &filename, vector<vector<double>> &img){
    ofstream out(filename);
    out << "P3" << endl << X << " " << Y << endl << "1" << endl;
    for(int i=0;i<X;i++)
        for(int j=0;j<Y;j++)
            out << (int)round(img[j][i]) << " " << (int)round(img[j][i]) << " " << (int)round(img[j][i]) << " ";
    out.close();
}

vector<double>* read_file(const string &filename){
    ifstream fin(filename);
    string format;
    int size; 
    fin >> format;
    fin >> X >> Y >> size; 
    N = X*Y;

    double p1, p2, p3;
    vector<double> *grayscale = new vector<double>();
    for(int i=0;i<N;i++){
        fin >> p1 >> p2 >> p3;
        grayscale->push_back((p1+p2+p3)/3);
    }
    fin.close();
    return grayscale;
}

bool edge(int i){
    return (i < X) || (!(i%X)) || !((i+1)%X) || (i > X*(Y-1));
}

void part2(){
    vector<double> *grayscale = read_file("image.ppm");
    delete grayscale;
}

int main(int argc, char *argv[]){
    part2();
}
