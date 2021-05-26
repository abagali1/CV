// Anup Bagali
// 05/26/2021
// Period 7
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>

#define DR 0.0174532

#define X 800
#define FRAMES 1000
#define OUTFILE "rotations.avi"

using namespace cv;
using namespace std;

const Scalar BLACK(0, 0, 0);
const Scalar WHITE(255, 255, 255);

const float eye[3] = {30, 30, 0};
const float vertices[8][3] = {
    {100, 100, 100},
    {100, -100, 100},
    {-100, 100, 100},
    {-100, -100, 100},
    {100, 100, -100},
    {100, -100, -100},
    {-100, 100, -100},
    {-100, -100, -100}
};
const float edges[12][2] = {
    {0, 1},
    {0, 2},
    {3, 1},
    {3, 2},
    {4, 5},
    {4, 6},
    {7, 5},
    {7, 6},
    {0, 4},
    {1, 5},
    {2, 6},
    {3, 7}
};

static inline void draw_circle(Mat m, Point c){
  circle(m, c, 5, WHITE, FILLED, LINE_8);
}

Mat render_frame(float theta){
    Mat buf(X, X, CV_8UC3, BLACK);
    vector<Matx<float, 3, 1>> rotated;
    for(int i=0;i<8;i++){
        float t = (2 - eye[0]/(vertices[i][0] - eye[0]));
        float u = eye[1] * (1-t) + t*vertices[i][1];
        float v = eye[2] * (1-t) + t*vertices[i][2];

        float ct = cosf(theta);
        float st = sinf(theta);

        Matx<float, 2, 1> m_r2(u, v);
        Matx<float, 3, 2> m_rXY(ct, -st, st, ct, 0, 0);
        Matx<float, 3, 1> m = m_rXY * m_r2;
        
        rotated.push_back(m);
        draw_circle(buf, Point(X/2 + m(0,0), X/2 + m(1,0)));
    }
    for(int i=0;i<12;i++){
        Point s = Point(X/2 + rotated[edges[i][0]](0,0), X/2 + rotated[edges[i][0]](1,0));
        Point e = Point(X/2 + rotated[edges[i][1]](0,0), X/2 + rotated[edges[i][1]](1,0));
        line(buf, s, e, WHITE, 2);
    }
    return buf;

}

int main(){
    float t = 0;
    VideoWriter vout(OUTFILE, cv::VideoWriter::fourcc('M', 'P', '4', '2'), 12, Size(X,X));
    for(int i=0;i<FRAMES;i++){
        vout.write(render_frame(t));
        t += 6*DR;
    }
    vout.release();

    return 0;
}
