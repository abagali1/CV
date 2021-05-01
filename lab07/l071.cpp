// Anup Bagali
// Period 7
// 04/30/2021
#include <bits/stdc++.h>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#define INFILE "image.jpg"
#define HT 100

using namespace std;
using namespace cv;

const Scalar RED(0, 0, 255);
const Scalar BLUE(255, 0, 0);
const Scalar GREEN(0, 255, 0);
const Scalar PURPLE(255, 0, 255);
const Scalar YELLOW(0, 255, 255);


int main(int argc, char** argv )
{
    Mat src, grayscale, edges;
    src = imread(INFILE, IMREAD_COLOR);
    cvtColor(src, grayscale, COLOR_BGR2GRAY);
    
    blur(grayscale, edges, Size(3,3));
    Canny(edges, edges, 70, HT, 3);

    int radius;
    vector<Vec3f> circles;
    HoughCircles(edges, circles, HOUGH_GRADIENT, 1, 165, 1, 30, 1, 165); // src, dest, --, scale, minDist, Canny Thres, Votes, minRad, maxRad
    for(size_t i=0;i<circles.size();i++){
        Vec3i c = circles[i];
        Point center = Point(c[0], c[1]);
        circle(src, center, 1, Scalar(0,255,0), 3, LINE_AA);
        radius = c[2];
        circle(src, center, radius, Scalar(255,0,0), 3, LINE_AA);
    }


    imwrite("./edges.png", edges);
    imwrite("./imagec.png", src);

    waitKey();
    
}