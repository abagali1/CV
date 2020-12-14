#include <chrono>
#include <bits/stdc++.h>

#define X 800
#define Y 800
#define li long int
#define uchar unsigned char
#define ulli unsigned long long 

#define INFILE "points.txt"
#define OUTFILE "results.txt"

using namespace std;
using namespace std::chrono;

class Point {
    public:
        double x, y;
        size_t z;
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

struct int_pair{
  ulli i1, i2;
  bool operator==(const int_pair& other) const{
    return i1==other.i1 && i2==other.i2;
  }
};

typedef struct int_pair ip;

struct hash_key
{
  std::size_t operator()(const int_pair& k) const
  {
    using std::size_t;
    using std::hash;
    using std::string;

    return hash<int>()(k.i1) ^ hash<int>()(k.i2);
  }
}; 
 

static inline double distance(Point p1, Point p2){
    return sqrt(pow(p2.x-p1.x, 2) + pow(p2.y-p1.y,2)); 
}

static inline int compare_x(Point p1, Point p2){ return p1.x < p2.x; }

static inline int compare_y(Point p1, Point p2){ return p1.y < p2.y; }

const int read_file(vector<Point>* points){
    ifstream fin (INFILE);
    string line;
    remove(OUTFILE);
    int i = 0;
    while(getline(fin, line)){
        int s = line.find(" ");
        points->push_back(Point(stod(line.substr(0, s), NULL), stod(line.substr(s+1,s), NULL)));
        i++;
    }
    return i;
}

void append_file(string n, dp d, li duration){
    FILE* fout = fopen(OUTFILE, "a");
    fprintf(fout, "%s (%0.17lf, %0.17lf) (%0.17lf, %0.17lf) ", n.c_str(), d.p1.x, d.p1.y, d.p2.x, d.p2.y);
    fprintf(fout, "Distance: %0.17e Time: %ld microseconds\n", d.d, duration);
    fclose(fout);
}

dp strip_closest(vector<Point> strip, double m_d, Point p1, Point p2, int lim = -1){
    double min = m_d;
    double d;
    Point t1 = p1;
    Point t2 = p2;
    int e = strip.size();
    if(lim > 0){
        sort(strip.begin(), strip.end(), compare_y);
        e = lim;
    }
    for(int i=0;i<strip.size();i++){
        for(int j=i+1;j<e;j++){
            d = distance(strip[i], strip[j]);
            if(d < min){
                min = d;
                t1 = strip[i];
                t2 = strip[j];
            }
        }
    }
    return (dp){.d=min, .p1=t1, .p2=t2};
    
}

dp brute_force(vector<Point>* points, int s, int e){
    Point p1, p2;
    double min = DBL_MAX;
    double d;
    for(int i=s;i<e;i++){
        for(int j=i+1;j<e;j++){
            d = distance((*points)[i], (*points)[j]);
            if(d < min){
                min = d;
                p1 = (*points)[i];
                p2 = (*points)[j];
            }
        }
    }
    return (dp){.d=min, .p1=p1, .p2=p2};
}

dp nlogn2_recur(vector<Point>* points, int s, int e, int lim = -1){
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
        if(abs((*points)[i].x - (*points)[mid].x) < d)
            strip.push_back(Point((*points)[i], 1));
    return strip_closest(strip, d, p1, p2, lim);
}

void shuffle(vector<Point>* points, int n){
    for (int i = n - 1; i > 0; i--){  
        int j = rand() % (i + 1); 
        swap((*points)[i], (*points)[j]);  
    } 
}

dp closest_randomized(vector<Point>* points, const int N){
    Point p1 = points->at(0);
    Point p2 = points->at(1);

    double md = distance(p1, p2);
    unordered_map<ip, Point, hash_key> dict;

    for(int i=0; i<N; i++){
        ip t1 = (ip){.i1=(ulli)(points->at(i).x/(md*0.5)), .i2=(ulli)(points->at(i).y/(md*0.5))};
        vector<Point> p;
        ulli one = 1/(md*0.5);
        for (ulli j = max(t1.i1-2, (ulli)0); j <= min(t1.i1+2, one); j++)
            for (ulli k = max(t1.i2-2, (ulli)0); k <= min(t1.i2+2, one); k++){
                ip c = {.i1=j, .i2=k};
                if(dict.find(c) != dict.end())
                    p.push_back(dict[c]);
            }
        int s = p.size();
        if(s){
            double tmd = distance(points->at(i), p.at(0));
            Point minPoint = p.at(0);
            for(int l=1; l<s; l++){
                double qd = distance(points->at(i), p.at(l));
                if(qd < tmd){
                    tmd = qd;
                    minPoint = p.at(l);
                }
            }
            ip ns = (ip){.i1=(ulli)(minPoint.x/(md*0.5)), .i2=(ulli)(minPoint.y/(md*0.5))};
            if(tmd < md){
                p1 = points->at(i);
                p2 = minPoint;
                md = tmd;
                dict.clear();
                dict[ns] = minPoint;
                for(int l=0; l<=i; l++){
                    ip pc = (ip){.i1=(ulli)(points->at(l).x/(md*0.5)), .i2=(ulli)(points->at(l).y/(md*0.5))};
                    dict[pc] = points->at(l);
                }
            }
        }
        else{
            dict[t1] = points->at(i);
        }
    }
    return (dp){.d=md, .p1=p1, .p2=p2};
}

void part3(vector<Point>* points, const int N){
    dp d;
    auto start_time = high_resolution_clock::now();
    {
        sort(points->begin(), points->end(), compare_x);
        d = nlogn2_recur(points, 0, N, 15);
    }
    auto end_time = high_resolution_clock::now();
    li duration = duration_cast<microseconds>(end_time - start_time).count();
    printf("%ld\n", duration);
    append_file("Full Recursive", d, duration);
}

void part4(vector<Point>* points, const int N){
    dp d;
    Point t = (*points)[0];
    auto start_time = high_resolution_clock::now();
    {
        shuffle(points, N);
        d = closest_randomized(points, N);
    }
    auto end_time = high_resolution_clock::now();
    li duration = duration_cast<microseconds>(end_time - start_time).count();
    printf("%ld\n", duration);
    append_file("Randomized", d, duration);
}

int main(int argc, char* argv[]) {
    srand(time(0));
    vector<Point> p3, p4;
    const int N = read_file(&p3);
    p4 = p3;

    part3(&p3, N);
    part4(&p4, N);

    // cout << ifstream(OUTFILE).rdbuf();

    return 0;
}