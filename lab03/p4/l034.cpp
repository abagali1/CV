#include <chrono>
#include <algorithm>
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

struct hash_pair { 
    template <class T1, class T2> 
    size_t operator()(const pair<T1, T2>& p) const
    { 
        auto hash1 = hash<T1>{}(p.first); 
        auto hash2 = hash<T2>{}(p.second); 
        return hash1 ^ hash2; 
    } 
}; 
 

static inline double distance(Point p1, Point p2){ return sqrt(pow(p2.x-p1.x, 2) + pow(p2.y-p1.y,2)); }

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

void part3(vector<Point>* points, const int N){
    dp d;
    auto start_time = high_resolution_clock::now();
    {
        sort(points->begin(), points->end(), compare_x);
        d = nlogn2_recur(points, 0, N, 15);
    }
    auto end_time = high_resolution_clock::now();
    li duration = duration_cast<microseconds>(end_time - start_time).count();
    append_file("Full Recursive", d, duration);
}

void shuffle(vector<Point>* points, int n){
    for (int i = n - 1; i > 0; i--){  
        int j = rand() % (i + 1); 
        swap((*points)[i], (*points)[j]);  
    } 
}

dp closest_randomized(vector<Point>* all_points, const int N){
    vector<Point> points = *all_points;
    Point p1 = points[0];
    Point p2 = points[1];
    double md = distance(p1, p2);
    unordered_map<pair<ulli, ulli>, Point, hash_pair> dict;
    for(int i=0;i<N;i++){
        pair<ulli, ulli> t1 = make_pair(points[i].x/(md*0.5), points[i].y/(md*0.5));
        vector<Point> p;
        ulli o = 1/(md*0.5);
        for(ulli j=max(t1.first-2, (ulli)0); j<=min(t1.first+2,o);j++){
            for(ulli k=max(t1.second-2, (ulli)0); k<=min(t1.second+2, o);k++){
                pair<ulli, ulli> c = make_pair(j, k);
                if(dict.find(c) != dict.end())
                    p.push_back(dict[c]);
            }
            ulli s = p.size();
            if(s){
                double tin = distance(points[i], p[0]);
                Point mp = p[0];
                for(int l=1; l<s; l++){
                    double qd = distance(points[i], p[l]);
                    if(qd < tin){
                        tin = qd;
                        mp = p[l];
                    }
                }
                pair<ulli, ulli> ns = make_pair(mp.x/(md*0.5), mp.y/(md*0.5));
                if(tin < md){
                    p1 = points[i];
                    p2 = mp;
                    md = tin;
                    dict.clear();
                    dict[ns] = mp;
                    for(int l=0;l<=i;l++){
                        pair<ulli, ulli> pc = make_pair(points[l].x/(md*0.5), points[l].y/(md*0.5));
                        dict[pc] = points[l];
                    }
                }
            }else{
                dict[t1] = points[i];
            }
        }
    }
    cout << md << endl;
    return (dp){.d=md, .p1=p1, .p2=p2};
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
    append_file("Randomized", d, duration);
}

int main(int argc, char* argv[]) {
    srand(time(0));
    // srand(1738114);
    vector<Point> p3, p4;
    const int N = read_file(&p3);
    p4 = p3;

    part3(&p3, N);
    part4(&p4, N);

    cout << ifstream(OUTFILE).rdbuf();

    return 0;
}