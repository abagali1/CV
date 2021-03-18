// Anup Bagali
// Period 7
// 03/08/2021

#include <bits/stdc++.h>

#define uchar unsigned char

#define INFILE "image.ppm"
#define GOUTFILE "imageg.ppm"
#define MOUTFILE "imagem.ppm"

#define T 300
#define E 1

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

vector<int>* read_file(){
    string line;
    ifstream fin(INFILE);
    
    getline(fin, line);
    getline(fin, line);
    
    int s = line.find(" ");
    X = stoi(line.substr(0, s));
    Y = stoi(line.substr(s+1));
    N = X*Y;

    vector<int> *gray = new vector<int>();

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
                int g = (t[0]+t[1]+t[2])/3;
                gray->push_back(g);
                fprintf(fout, "%d %d %d ", g, g, g);
                j = 0;
                i++;
            }
        }
        fprintf(fout, "\n");
    }
    fclose(fout);
    return gray;
}

void write_result(vector<int> *colors){
    FILE* fout = fopen(MOUTFILE, "w");
    fprintf(fout, "P3\n%d %d\n1\n", X, Y);
    for(int i=0;i<N;i++)
        fprintf(fout, "%d %d %d\n", colors->at(i), colors->at(i), colors->at(i));
    fclose(fout);
}

double dot(const int p[9], const int op[9]){
    return p[0]*op[0] + p[1]*op[1] + p[2]*op[2] + p[3]*op[3] + p[4]*op[4] + p[5]*op[5] + p[6]*op[6] + p[7]*op[7] + p[8]*op[8];
}

bool edge(int i){
    return (i < X) || (!(i%X)) || !((i+1)%X) || (i > X*(Y-1));
}}

void sobel_threshold(vector<int> &grayscale){
    vector<int> *gradient = new vector<int>(N);
    for(int i=0;i<N;i++){
        if(edge(i)){
            gradient->at(i) = 0;
        }
        else{
            int grad[9] = {
                grayscale[i-X-1], grayscale[i-X], grayscale[i-X+1],
                grayscale[i-1]  , grayscale[i]  , grayscale[i+1]  ,
                grayscale[i+X-1], grayscale[i+X], grayscale[i+X+1],
            };
            gradient->at(i) = (int)(sqrt(pow(dot(grad, SX), 2) + pow(dot(grad, SY), 2)) > T);
        }
    }
    write_result(gradient);
    delete gradient;
}

void part1(){
    vector<int> *gray = read_file();
    sobel_threshold(*gray);
    delete gray;
}


int main(int argf, char* argc[]){
    part1();
}
