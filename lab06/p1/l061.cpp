// Anup Bagali
// 04/11/12
// Period 7
#include <bits/stdc++.h>

#define uchar unsigned char

#define INFILE "image.ppm"
#define FOUT "imagef.ppm"
#define VOUT "imagev.ppm"
#define OUTFILE "imageCC.ppm"

#define RD 57.2957795131

#define T1 70
#define T2 120
#define CT 0

using namespace std;

class Point {
    public:
        double x, y;
    Point(){
        this->x = (double)rand()/(double)RAND_MAX;
        this->y = (double)rand()/(double)RAND_MAX;
    }
    Point(double _x, double _y): x(_x), y(_y){};
    Point(Point p1, double scalar): x(p1.x*scalar), y(p1.y*scalar){};
    Point(Point p1, double s_x, double s_y): x(p1.x*s_x), y(p1.y*s_y){};
    inline double distance(Point &p){ return sqrt(pow(this->x-p.x, 2) + pow(this->y-p.y, 2)); }
    bool operator == (const Point &p) const{
        return this->x == p.x && this->y == p.y;
    }
    bool operator != (const Point &p) const{
        return this->x != p.x || this->y != p.y;
    }
    friend ostream& operator << (ostream &out, const Point& p) {
        out << fixed << setprecision(17) << p.x << "  " << p.y;
        return out;
    }
    friend istream& operator>>(istream &input, Point &p){
      input >> p.x >> p.y;
      return input;
    }
    const double& operator[](int i) const{
        if(i < 0 || i >=2)
            throw invalid_argument("point range out of bounds");
        return i == 0 ? this->x : this->y;
    }
};

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

void write_ppm(const string &filename, vector<int> &img, int size = 1){
    ofstream out(filename);
    out << "P3" << endl << X << " " << Y << endl << size << endl;
    int p;
    for(int d: img){
        p = min(d, size);
        out << p << " " << p << " " << p << " ";
    }
    out.close();
}

void write_ppm(const string &filename, vector<vector<int>> &img, int size = 1){
    ofstream out(filename);
    out << "P3" << endl << X << " " << Y << endl << size << endl;
    int p;
    for(int i=0;i<X;i++){
        for(int j=0;j<Y;j++){
            p = min(img[j][i], size);
            out << p << " " << p << " " << p << " ";
        }
    }
    out.close();
}

vector<int>* read_file(const string &filename){
    ifstream fin(filename);
    if(fin.fail()){
        cout << filename << " not found" << endl;
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

void draw_line(vector<vector<int>> &tally, Point p1, Point p2){ // all cases
    int dx = p2.x - p1.x;
    int dy = p2.y - p1.y;
    
    int i1 = 0;
    int i2 = 0;
    int j1 = 0;
    int j2 = 0;
    i1 = dx > 0 ? 1 : -1;
    i2 = dx > 0 ? 1 : -1;
    j1 = dy > 0 ? 1 : -1;
    
    int i = abs(dx);
    int j = abs(dy);
    if(j>i){ // find DA
        swap(i,j);
        j2 = dy > 0 ? 1 : -1;
        i2 = 0;
    }
    
    int e = j/i;
    int x0 = p1.x; // keep the Point ints intact
    int y0 = p1.y;
    for(int k=0;k<=i;k++){
        if(x0 < X && x0 >=0 && y0 < Y && y0 >= 0)
            tally[x0][y0] += 1;
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

vector<int>* nms(vector<int> &gradient, vector<int> &angles){
    vector<int> *n = new vector<int>(N, 0);
    int g, t, n1, n2;
    for(int i=0;i<N;i++){
        if(!edge(i)){
            g = gradient[i];
            t = closest_angle(angles[i] * RD);
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


void hysteresis(vector<int> &hyst, int i){
    if(i < 0 || i > N || hyst[i] == 0)
        return;
    
    if(hyst[i] != 3){
        hyst[i] = 3;
        for(int j=i-X-1;j<=i+X+1;j++){
            if(j != i)
                hysteresis(hyst, j);
            else
                return;
        }
    }
}

void combine(vector<int> &suppressed, vector<int> &hyst, vector<int> &edges){
    ofstream fout(FOUT);
    fout << "P3" << endl << X << " " << Y << endl << 1 << endl;
    int p;
    for(int i=0;i<N;i++){
        p = (int)(suppressed[i] && hyst[i]);
        edges[i] = p;
        fout << p << " " << p << " " << p << " ";
    }
    fout.close();
}

vector<int> detect_edges(vector<int> &grayscale, vector<int> &angles){
    vector<int> gradient(N, 0);
    vector<int> hyst(N, 0);
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
            angles[i] = atan2(gy, gx);
            hyst[i] = mag > T2 ? 2 : (mag > T1 ? 1 : 0);
        }
    }
    vector<int> *suppressed = nms(gradient, angles);
    vector<int> edges(N, 0);

    for(int i=0;i<N;i++)
        if(hyst[i] == 2)
            hysteresis(hyst, i);

    combine(*suppressed, hyst, edges);
    delete suppressed;

    return edges;
}

void centers(vector<int> &edges, vector<int> &angles){
    double m;
    Point s, e;
    int k;
    vector<vector<int>> tally(X, vector<int>(Y, 0));
    for(int i=0;i<X;i++){
        for(int j=0;j<Y;j++){
            k = j*X+i;
            if(edges[k]){
                m = tan(angles[k]);
                s = Point((j/m)+i, 0);
                e = Point(((-Y+j)/m) + i, Y);
                draw_line(tally, s, e);
            }
        }
    }
}


void part1(){
    vector<int> *grayscale = read_file(INFILE);
    
    vector<int> angles(N, 0);
    vector<int> edges = detect_edges(*grayscale, angles);
    cout << "got edges" << endl;
    centers(edges, angles);
    cout << "got centers" << endl;

    delete grayscale;
}

int main(int argc, char *argv[]){
    part1();
}