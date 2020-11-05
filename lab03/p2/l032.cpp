// Anup Bagali
// 10/27/2020
// Period 7
#include <bits/stdc++.h>

#define amt 50
#define X 800
#define Y 800
#define uchar unsigned char

using namespace std;

class Color{
    public:
        uchar r, g, b;
        Color(){
            this->r=0;
            this->g=0;
            this->b=0;
        };
        Color(uchar r, uchar g, uchar b){
            this->r=r;
            this->g=g;
            this->b=b;
        }
        void print(){
            printf("R: %d, G: %d, B: %d\n", this->r, this->g, this->b);
        }
};

const Color BLACK(0, 0, 0);
const Color WHITE(255, 255, 255);
const Color RED(255, 0, 0);

class Point {
    public:
        double x, y;
    Point(){
        this->x = 0;
        this->y = 0;
    }
    Point(double x, double y){
        this->x = x;
        this->y = y;
    }
    Point(Point p1, double scalar){
        this->x = p1.x*scalar;
        this->y = p1.y*scalar;
    }
    Point(Point p1, double s_x, double s_y){
        this->x = p1.x*s_x;
        this->y = p1.y*s_y;
    }
    void print(){
        printf("X: %e. Y: %e\n", this->x, this->y);
    }
};

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
    while(getline(fin, line)){
        int s = line.find(" ");
        points.push_back(Point(stod(line.substr(0, s), NULL), stod(line.substr(s+1,s), NULL)));
    }
    return points;
}


void closest_point_n2(list<Point> points, Point* p1, Point* p2){
    double min_distance = DBL_MAX;
    for(list<Point>::iterator i = points.begin(); i != points.end(); ++i){
        for(list<Point>::iterator j = i; j != points.end(); ++j){
            if(i != j){
                double d = distance(*i, *j);
                if(d < min_distance){
                    min_distance = d;
                    *p1 = *i;
                    *p2 = *j;
                }
            }
        }
    }
}

double nlogn2_recur(vector<Point> points, int n){
    int mid = n/2;
    Point midp = points[mid];

    double d1 = nlogn2_recur(points, mid);
    double d2 = nlogn2_recur(vector<Point>(points.begin() + mid, points.end()), n-mid);

    double min = d1 < d2 ? d1 : d2;
    vector<Point> tmp;
    for(int i=0;i<n;i++)
        if(abs(points[i].x - midp.x) < min)
            tmp.push_back(points[i]);

}

void closest_point_nlogn2(vector<Point> points, Point* p1, Point* p2){
    if(points.size() <= 3)
        return closest_point_n2(list<Point>(points.begin(), points.end()), p1, p2);
    sort(points.begin(), points.end(), compare_x);

}

time_t part1(){
    srand(time(0));
    FILE* fout;
    list<Point> points;
    Color** colors = new Color*[X];
    for(int i=0;i<X;i++){
        colors[i] = new Color[Y];
        for(int j=0;j<Y;j++){
            colors[i][j] = WHITE;
        }
    }
    fout = fopen("points.txt", "w");
    for(int i=0;i<amt;i++){
        Point p(rand()/(double)RAND_MAX, rand()/(double)RAND_MAX);
        set_pixel(colors, p.x*X, p.y*Y, BLACK);
        draw_circle(colors, Point(p, X), 2.0, BLACK);
        points.push_back(p);
        fprintf(fout, "%0.25lf %0.25lf\n", p.x, p.y);
    }
    fclose(fout);

    time_t start_time = time(0);
    Point p1, p2;
    closest_point_n2(points, &p1, &p2);
    time_t end_time = time(0);
    
    p1 = Point(p1, X);
    p2 = Point(p2, X);
    set_pixel(colors, p1, RED);
    draw_circle(colors, p1, 2.0, RED);

    set_pixel(colors, p2, RED);
    draw_circle(colors, p2, 2.0, RED);
    
    fout = fopen("points.ppm", "w");
    fprintf(fout, "P3\n%d %d\n255\n", X, Y);
    for(int i=0;i<X;i++){
        for(int j=0;j<Y;j++){
            fprintf(fout, "%d %d %d\n", colors[i][j].r, colors[i][j].g, colors[i][j].b);
        }
    }
    fclose(fout);
    delete[] colors;
    return end_time - start_time;
}

time_t part2(){
    vector<Point> points = parse_file();
    Point p1, p2;
    closest_point_nlogn2(points, &p1, &p2);
    return time(0);
}

int main(){
    time_t p1_t = part1();
    time_t p2_t = part2();
    return 0;
}
