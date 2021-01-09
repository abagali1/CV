// Anup Bagali
// Period 7
// 12/16/2020
#include <bits/stdc++.h>

#define uchar unsigned char

#define K 4
#define INFILE "peppers.ppm"
#define OUTFILE "clusters.ppm"

using namespace std;

class Point {
    public:
        double x, y;
    Point(){
        this->x = rand()/(double)RAND_MAX;
        this->y = rand()/(double)RAND_MAX;
    }
    Point(double _x, double _y): x(_x), y(_y){};
    Point(Point p1, double scalar): x(p1.x*scalar), y(p1.y*scalar){};
    Point(Point p1, double s_x, double s_y): x(p1.x*s_x), y(p1.y*s_y){};
    inline double distance(Point& p){ return sqrt(pow(this->x-p.x, 2) + pow(this->y-p.y, 2)); }
    inline void print(){ printf("X: %e. Y: %e\n", this->x, this->y); };
    bool operator == (const Point& p) const{
        return this->x == p.x && this->y == p.y;
    }
    bool operator != (const Point& p) const{
        return this->x != p.x || this->y != p.y;
    }
};

class Color{
    public:
        uchar r, g, b;
        Color(): r(255), g(255), b(255){};
        Color(uchar _r, uchar _g, uchar _b): r(_r), g(_g), b(_b){};
        inline double distance(Color& c){ return sqrt(pow(this->r-c.r, 2)) + sqrt(pow(this->g-c.g,2)) + sqrt(pow(this->b-c.b, 2)); }
        inline void print(){ printf("R: %d, G: %d, B: %d\n", this->r, this->g, this->b); };
        
        bool operator==(const Color& c) const{
            return this->r == c.r && this->g == c.g && this->b == c.b;
        }
    
        bool operator!=(const Color& c) const{
            return this->r != c.r || this->g != c.g || this->b != c.b;
        }
};


class ColorHash { 
    public: 
        size_t operator()(const Color& p) const
        { 
            return hash<uchar>()(p.r) ^ hash<uchar>()(p.g) ^ hash<uchar>()(p.b);
        } 
}; 

int X, Y, N;
const Color WHITE(255,255,255);
const Color BLACK(0,0,0);

static inline void set_pixel(Color** c, double x, double y, Color color){
    if(x >= X || x < 0 || y >= Y || y < 0){
        return;
    }
    c[(int)x][(int)y] = color;
}

static inline void set_pixel(Color** c, Point p, Color color){
    set_pixel(c, p.x, p.y, color);
}

Color** read_file(vector<Color>& colors){
    string line;
    ifstream fin(INFILE);
    remove(OUTFILE);
    
    getline(fin, line);
    getline(fin, line);
    
    X = stoi(line.substr(0, 3));
    Y = stoi(line.substr(4, 3));
    N = X*Y;
    
    getline(fin, line);
    
    Color** c = new Color*[X];
    for(int i=0;i<X;i++)
        c[i] = new Color[Y];
    
    int i = 0;
    int j = 0;
    while(getline(fin, line)){
        istringstream is(line);
        string part;
        uchar t[3];
        while(getline(is, part, ' ')){
            t[j] = stoi(part);
            j++;
            if(j == 3){
                Color tmp = Color(t[0], t[1], t[2]);
                c[i/Y][i%Y] = tmp;
                colors.push_back(tmp);
                j = 0;
                i++;
            }
        }
    }
    return c;
}

void write_file(Color** colors){
    FILE* fout = fopen(OUTFILE, "w");
    fprintf(fout, "P3\n%d %d\n255\n", X, Y);
    for(int i=0;i<X;i++){
        for(int j=0;j<Y;j++){
            fprintf(fout, "%d %d %d\n", colors[i][j].r, colors[i][j].g, colors[i][j].b);
        }
    }
    fclose(fout);
}


Color mean_for_pixel(Color means[K], Color p){
    Color m = means[0];
    double md = p.distance(means[0]);
    for(int i=1;i<K;i++){
        double d = p.distance(means[i]);
        if(md > d){
            md = d;
            m = means[i];
        }
    }
    return m;
}

bool reorganize(unordered_map<Color, vector<Color>, ColorHash>* previous, Color means[K], vector<Color>* points){
    Color new_means[K];
    for(int i=0;i<K;i++){
        vector<Color> tmp_points = previous->at(means[i]); 
        int n = tmp_points.size();
        double r = 0;
        double g = 0;
        double b = 0;
        for(int j=0;j<n;j++){
            r += tmp_points[j].r;
            g += tmp_points[j].g;
            b += tmp_points[j].b;
        }
        new_means[i] = Color(r/n, g/n, b/n);
    }
    previous->clear();
    for(int i=0;i<N;i++){
        Color p = points->at(i);
        (*previous)[mean_for_pixel(new_means, p)].push_back(p); 
    }
    bool organized = true;
    for(int i=0;i<K;i++){
        if(means[i] != new_means[i])
            organized = false;
        means[i] = new_means[i];
    }
    return organized;
}

// void part1(){
//     Point means[K];
//     Color palette[K];
//     Color** colors = new Color*[X];
//     for(int i=0;i<X;i++){
//         colors[i] = new Color[Y];
//     }
//     vector<Point>* points = new vector<Point>(N);
//     gen_palette(palette);

//     unordered_map<Point, vector<Point>, PointHash> organized_means;
//     for(int i=0;i<N;i++){
//         organized_means[mean_for_point(means, points->at(i))].push_back(points->at(i));
//     }
    
//     bool organized = false;
//     do{
//         organized = reorganize(&organized_means, means, points);
//     }while(!organized);
    
//     for(int i=0;i<K;i++){
//         vector<Point> points = organized_means[means[i]];
//         for(int j=0;j<points.size();j++){
//             draw_circle(colors, Point(points[j], X, Y), 2.0, palette[i]);
//         }
//         draw_circle(colors, Point(means[i], X, Y), 3.0, BLACK);
//     }

//     write_file(colors);

//     delete[] colors;
//     delete points;
// }


void part2(){
    vector<Color>* pixels = new vector<Color>;
    Color** colors = read_file(*pixels);
    Color means[K];
    for(int i=0;i<K;i++){
        means[i] = pixels->at(i);
    }
    
    unordered_map<Color, vector<Color>, ColorHash> organized_means;
    for(int i=0;i<N;i++){
        organized_means[mean_for_pixel(means, pixels->at(i))].push_back(pixels->at(i));
    }
    
    bool organized = false;
    do{
        organized = reorganize(&organized_means, means, pixels);
    }while(!organized);
    
    
    cout << "organized" << endl;
    for(int i=0;i<X;i++){
        for(int j=0;j<Y;j++){
            Color c = colors[i][j];
            for(const auto it: organized_means){
                if(find(it.second.begin(), it.second.end(), c) != it.second.end()){
                    colors[i][j] = it.first;
                    break;
                }
            }
        }
    }
    write_file(colors);
    
   delete[] colors;
   delete pixels;
}




int main(int argv, char* argc[]){
    srand(time(0));
    part2();
}