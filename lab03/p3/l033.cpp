// Anup Bagali
// 10/27/2020
// Period 7
#include <chrono>
#include <bits/stdc++.h>

#define X 800
#define Y 800
#define uchar unsigned char
#define li long int

using namespace std;
using namespace std::chrono;

class Point {
    public:
        double x, y;
    Point(): x(0), y(0){};
    Point(double _x, double _y): x(_x), y(_y){};
    Point(Point p1, double scalar): x(p1.x*scalar), y(p1.y*scalar){};
    Point(Point p1, double s_x, double s_y): x(p1.x*s_x), y(p1.y*s_y){};
    inline int operator==(const Point& p1){
        return p1.x==this->x && p1.y==this->y;
    }
    inline void print(){
        printf("X: %e. Y: %e\n", this->x, this->y);
    };
};

typedef struct {
    double d;
    Point p1, p2;
} dp;

class Color{
    public:
        uchar r, g, b;
        Color(): r(0), g(0), b(0){};
        Color(uchar _r, uchar _g, uchar _b): r(_r), g(_g), b(_b){};
        inline void print(){
            printf("R: %d, G: %d, B: %d\n", this->r, this->g, this->b);
        };
};

const Color BLACK(0, 0, 0);
const Color WHITE(255, 255, 255);
const Color RED(255, 0, 0);


static inline void set_pixel(Color** c, double x, double y, Color color){
    if(x >= X || x < 0 || y >= Y || y < 0){
        return;
    }
    c[(int)x][(int)y] = color;
}

static inline void set_pixel(Color** c, Point p, Color color){
    set_pixel(c, p.x, p.y, color);
}

static inline double distance(Point p1, Point p2){
    return sqrt(pow(p2.x-p1.x, 2) + pow(p2.y-p1.y,2));
}

static inline int compare_x(Point p1, Point p2){
    return p1.x < p2.x;
}

static inline int compare_y(Point p1, Point p2){
    return p1.y < p2.y;
}

void draw_circle(Color** c, Point center, double r, Color color){
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

vector<Point> parse_file(){
    vector<Point> points;
    ifstream fin ("points.txt");
    string line;
    remove("results.txt");
    while(getline(fin, line)){
        int s = line.find(" ");
        points.push_back(Point(stod(line.substr(0, s), NULL), stod(line.substr(s+1,s), NULL)));
    }
    return points;
}

void create_ppm(vector<Point> points, dp d){
    Color** colors = new Color*[X];
    Color c;
    for(int i=0;i<X;i++){
        colors[i] = new Color[Y];
        for(int j=0;j<Y;j++)
            colors[i][j] = WHITE;
    }
    
    for(int i=0;i<points.size();i++){
        c = (points[i] == d.p1 || points[i] == d.p2) ? RED : BLACK;
        set_pixel(colors, Point(points[i], X, Y), c);
        draw_circle(colors, Point(points[i], X, Y), 2.0, c);
    }

    FILE* fout = fopen("points.ppm", "w");
    fprintf(fout, "P3\n%d %d\n255\n", X, Y);
    for(int i=0;i<X;i++){
        for(int j=0;j<Y;j++){
            fprintf(fout, "%d %d %d\n", colors[i][j].r, colors[i][j].g, colors[i][j].b);
        }
    }
    fclose(fout);
    delete[] colors;
}

void append_file(string n, dp d, li duration){
    FILE* fout = fopen("results.txt", "a");
    fprintf(fout, "%s: (%0.25lf, %0.25lf) (%0.25lf, %0.25lf) ", n.c_str(), d.p1.x, d.p1.y, d.p2.x, d.p2.y);
    fprintf(fout, "Distance: %0.25lf Time: %ld microseconds\n", d.d, duration);
    fclose(fout);
}

dp strip_closest(vector<Point> strip, double d, Point p1, Point p2, int lim = -1){
    double min = d;
    Point t1;
    Point t2;
    int it;
    if(lim != -1){
        sort(strip.begin(), strip.end(), compare_y);
        it = 0;
    }
    for(vector<Point>::iterator i=strip.begin();i<strip.end();++i){
        for(vector<Point>::iterator j=i; j<strip.end() && abs(j->y - i->y) < min; ++j){
            if(i != j){
                double t_d = distance(*i, *j);
                if(t_d < min){
                    min = t_d;
                    t1 = Point(i->x, i->y);
                    t2 = Point(j->x, j->y);
                }
                it++;
                if(lim != -1 && it >= lim)
                    return (dp){.d=min, .p1=t1, .p2=t2};
            }
        }
    }
    return (dp){.d =min, .p1=t1, .p2=t2};
    
}

dp brute_force(vector<Point> points, int s, int e){
    Point p1, p2;
    double min = DBL_MAX;
    for(vector<Point>::iterator i=points.begin()+s;i<points.begin()+e+1;++i){
        for(vector<Point>::iterator j=i;j<points.begin()+e+1;j++){
            if(i != j){
                double d = distance(*i, *j);
                if(d < min){
                    min = d;
                    p1 = *i;
                    p2 = *j;
                }
            }
        }
    }
    return (dp){.d=min, .p1=p1, .p2=p2};
}

dp nlogn2_recur(vector<Point> points, int s, int e, int lim = -1){
    if((e-s) <= 3){
        return brute_force(points, s, e);
    }

    int mid = s + (e-s)/2;

    dp d1 = nlogn2_recur(points, s, mid);
    dp d2 = nlogn2_recur(points, mid, e);

    double d;
    Point p1, p2;
    if(d1.d < d2.d){
        d = d1.d;
        p1 = d1.p1;
        p2 = d1.p2;
    }else{
        d = d2.d;
        p1 = d2.p1;
        p2 = d2.p2;
    }
    vector<Point> strip;
    for(int i=s; i<e; i++)
        if(abs(points[i].x - points[mid].x) < d)
            strip.push_back(Point(points[i], 1));
    dp ss = strip_closest(strip, d, p1, p2, lim);
    return ss.d < d ? ss : (dp){.d=d, .p1=p1, .p2=p2};
}

void part1(vector<Point> points){
    dp d;
    auto start_time = high_resolution_clock::now();
    {
        d = brute_force(points, 0, points.size());
    }
    auto end_time = high_resolution_clock::now();
    li duration = duration_cast<microseconds>(end_time - start_time).count();
    append_file("Brute Force", d, duration);
}

 void part2(vector<Point> points){
    dp d;
    auto start_time = high_resolution_clock::now();
    {
        sort(points.begin(), points.end(), compare_x);
        d = nlogn2_recur(points, 0, points.size());
    }
    auto end_time = high_resolution_clock::now();
    li duration = duration_cast<microseconds>(end_time - start_time).count();
    append_file("Initial Recursive", d, duration);    
}

void part3(vector<Point> points){
    dp d;
    auto start_time = high_resolution_clock::now();
    {
        sort(points.begin(), points.end(), compare_x);
        d = nlogn2_recur(points, 0, points.size(), 15);
    }
    auto end_time = high_resolution_clock::now();
    li duration = duration_cast<microseconds>(end_time - start_time).count();
    append_file("Full Recursive", d, duration);
    create_ppm(points, d);
}

int main(){
    vector<Point> p1 = parse_file();
    vector<Point> p2(p1);
    vector<Point> p3(p1);
    // part1(p1);
    part2(p2);
    part3(p3);
    return 0;
}
