// Anup Bagali
// Period 7
// 03/18/2021

#include <bits/stdc++.h>

#define uchar unsigned char

#define INFILE "image.ppm"
#define NOUT "image1.ppm"
#define HOUT "image2.ppm"
#define FOUT "imagef.ppm"

#define RD 57.2957795131

#define T1 70
#define T2 120

using namespace std;

int X, Y, N;

const int SX[9] = {1, 0, -1, 2, 0, -2, 1, 0, -1};
const int SY[9] = {1, 2, 1, 0, 0, 0, -1, -2 ,-1};

const int ANGLES[5] = {0, 45, 90, 135, 180};

static inline bool edge(int i){
    return (i < X) || (!(i%X)) || !((i+1)%X) || (i > X*(Y-1));
}

static inline int dot(const int p[9], const int op[9]){
    return p[0]*op[0] + p[1]*op[1] + p[2]*op[2] + p[3]*op[3] + p[4]*op[4] + p[5]*op[5] + p[6]*op[6] + p[7]*op[7] + p[8]*op[8];
}

int closest_angle(const int &theta){
    int t = abs(theta);
    int min_dist = 181;
    int angle, d;
    for(const int &i: ANGLES){
        d = abs(t-i);
        if(d < min_dist){
            min_dist = d;
            angle = i;
        }
    }
    return ((theta > 0) - (theta < 0)) * angle;
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
    if(fin.fail()){
        cout << "image.ppm not found" << endl;
        return NULL;
    }

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

vector<int>* nms(vector<int> &gradient, vector<int> &angles){
    vector<int> *n = new vector<int>(N, 0);
    int g, t, n1, n2;
    for(int i=0;i<N;i++){
        if(!edge(i)){
            g = gradient[i];
            t = angles[i];
            if(!(t % 180)){
                n1 = gradient[i-1];
                n2 = gradient[i+1];
            }else if(t == 45 || t == -135){
                n1 = gradient[i-X+1];
                n2 = gradient[i+X-1];
            }else if(t == -45 || t == 135){
                n1 = gradient[i-X-1];
                n2 = gradient[i+X+1];
            }else{
                n1 = gradient[i-X];
                n2 = gradient[i+X];
            }
            n->at(i) = (int)((g >= n1) && (g >= n2));
        }else{
            n->at(i) = 0;
        }
    }
    return n;
}

void gradient(vector<int> &grayscale){
    vector<int> gradient(N, 0);
    vector<int> hyst(N, 0);
    vector<int> angles(N, 0);
    int gx, gy;
    double mag;
    for(int i=X+1;i<N-X;i++){
        if(!edge(i)){
            int grad[9] = {
                grayscale[i-X-1], grayscale[i-X], grayscale[i-X+1],
                grayscale[i-1]  , grayscale[i]  , grayscale[i+1]  ,
                grayscale[i+X-1], grayscale[i+X], grayscale[i+X+1],
            };
            gx = dot(grad, SX);
            gy = dot(grad, SY);
            mag = sqrt(pow(gx, 2) + pow(gy, 2));
            gradient[i] = (int)mag;
            angles[i] = closest_angle(atan2(gy, gx) * RD);
            hyst[i] = mag > T2 ? 2 : (mag > T1 ? 1 : 0);
        }
    }
    vector<int> *suppressed = nms(gradient, angles);


    write_ppm(NOUT, *suppressed);
    
    delete suppressed;
}

void part2(){
    vector<int> *grayscale = read_file("image.ppm");
    gradient(*grayscale);
    
    delete grayscale;
}

int main(int argc, char *argv[]){
    part2();
}

