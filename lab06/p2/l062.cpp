// Anup Bagali
// 04/11/21
// Period 7
#include <bits/stdc++.h>

#define uchar unsigned char

#define INFILE "image.ppm"
#define FOUT "imagef.ppm"
#define VOUT "imagev.ppm"
#define OUTFILE "imageCC.ppm"

#define RD 57.2957795131

#define T1 70
#define T2 100
#define CT 75

#define RO 10
#define RF 80

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

class Color{
    public:
        uchar r, g, b;
        Color(): r(255), g(255), b(255){};
        Color(uchar _r, uchar _g, uchar _b): r(_r), g(_g), b(_b){};
        const bool operator==(const Color &c) const{
            return this->r == c.r && this->g == c.g && this->b == c.b;
        }

        const bool operator!=(const Color &c) const {
            return this->r != c.r || this->g != c.g || this->b != c.b;
        }
        
        friend ostream &operator<<(ostream &out, const Color &c){
            out << (int)c.r << " " << (int)c.g << " " << (int)c.b << " ";
            return out;
        }
};

int X, Y, N;
int MT = 0;

const int SX[9] = {1, 0, -1, 2, 0, -2, 1, 0, -1};
const int SY[9] = {1, 2, 1, 0, 0, 0, -1, -2 ,-1};

const int ANGLES[5] = {0, 45, 90, 135, 180};

const Color RED(255, 0, 0);

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
    for(size_t i=0;i<img[0].size();i++){
        for(size_t j=0;j<img.size();j++){
            p = min(img[j][i], size);
            out << p << " " << p << " " << p << " ";
        }
    }
    out.close();
}

void write_ppm(const string &filename, vector<Color> &img, int size = 255){
    ofstream out(filename);
    out << "P3" << endl << X << " " << Y << endl << size << endl;
    for(const Color &d: img){
        out << d;
    }
    out.close();
}

vector<int>* read_file(const string &filename, vector<Color> *orig){
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
        orig->push_back(Color(p1, p2, p3));
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
        if(x0 < X && x0 >=0 && y0 < Y && y0 >= 0){
            tally[x0][y0] += 1;
            if(tally[x0][y0] > MT)
                MT = tally[x0][y0];
        }
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

static inline void set_pixel(vector<Color> &c, double x, double y, Color color){
    if(x >= X || x < 0 || y >= Y || y < 0){
        return;
    }
    c[y*X+x] = color;
}

static inline void set_pixel(vector<Color> &c, Point p, Color color){
    set_pixel(c, p.x, p.y, color);
}

static inline int check_pixel(vector<int> &edges, double x, double y){
    return (x >= X || x < 0 || y >= Y || y < 0) ? 0 : (int)(edges[y*X+x] > 0);
}

void draw_circle(vector<Color> &c, Point center, double r, Color color){
    set_pixel(c, center, color);
    int x = 0;
    int y = r;
    int xmax = (int) (r * 0.70710678); 
    int y2 = y * y;
    int y2_new = y2;
    int ty = (2 * y) - 1;
    for (x = 0; x <= xmax; x++) {
        if ((y2 - y2_new) >= ty) {
            y2 -= ty;
            y -= 1;
            ty -= 2;
        }
        set_pixel (c, center.x+x, center.y+y, color);
        set_pixel (c, center.x+x, center.y-y, color);
        
        set_pixel (c, center.x-x, center.y+y, color);
        set_pixel (c, center.x-x, center.y-y, color);
        
        set_pixel (c, center.x+y, center.y+x, color);
        set_pixel (c, center.x+y, center.y-x, color);
        
        set_pixel (c, center.x-y, center.y+x, color);
        set_pixel (c, center.x-y, center.y-x, color);
        
        y2_new -= (2 * x) - 3;
    }
}

int trace_circle(vector<int> &edges, Point center, double r){
    int x = 0;
    int y = r;
    int xmax = (int) (r * 0.70710678); 
    int y2 = y * y;
    int y2_new = y2;
    int ty = (2 * y) - 1;
    int total = 0;
    for (x = 0; x <= xmax; x++) {
        if ((y2 - y2_new) >= ty) {
            y2 -= ty;
            y -= 1;
            ty -= 2;
        }

        total += check_pixel (edges, center.x+x, center.y+y);
        total += check_pixel (edges, center.x+x, center.y-y);
        
        total += check_pixel (edges, center.x-x, center.y+y);
        total += check_pixel (edges, center.x-x, center.y-y);
        
        total += check_pixel (edges, center.x+y, center.y+x);
        total += check_pixel (edges, center.x+y, center.y-x);
        
        total += check_pixel (edges, center.x-y, center.y+x);
        total += check_pixel (edges, center.x-y, center.y-x);
        
        y2_new -= (2 * x) - 3;
    }
    return total;
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

vector<int>* nms(vector<int> &gradient, vector<double> &angles){
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

void vote_line(vector<vector<int>> &tally, int x, int y, double m){
    Point s;
    Point e;
    if(abs(m) < 1e-4){
        s = Point(0, y);
        e = Point(X, y);
    }else{
        s = Point((y/m)+x, 0);
        e = Point(((-Y+y)/m)+x, Y);
    }
    draw_line(tally, s, e);
}

vector<int> detect_edges(vector<int> &grayscale, vector<double> &angles){
    vector<int> hyst(N, 0);
    vector<int> gradient(N, 0);
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

void part2(){
    vector<Color> *orig = new vector<Color>();
    vector<int> *grayscale = read_file(INFILE, orig);

    vector<double> angles(N, 0);
    vector<int> edges = detect_edges(*grayscale, angles);
    
    vector<vector<int>> tally(X, vector<int>(Y, 0));
    for(int i=0;i<X;i++){
        for(int j=0;j<Y;j++){
            int k = j*X+i;
            if(edges[k])
                vote_line(tally, i, j, -tan(angles[k]));
        }
    }
    write_ppm(VOUT, tally, MT);

    int k;
    int radii[RF-RO];
    for(int i=0;i<X;i++){
        for(int j=0;j<Y;j++){
            k = j*X+i;
            if(tally[i][j] > CT){
                for(int x=RO;x<RF;x++)
                    radii[x] = trace_circle(edges, Point(i, j), x);
                
                orig->at(k) = RED;
                for(int s=0;s<5;s++)
                    draw_circle(*orig, Point(i, j), s, RED);
            }
        }
    }

    write_ppm(OUTFILE, *orig);


    delete orig;
    delete grayscale;
}

int main(int argc, char *argv[]){
    part2();
}