// Anup Bagali
// Period 7
// 04/30/2021
#include <bits/stdc++.h>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#define INFILE "image.jpg"
#define LT 80 
#define HT 120

using namespace std;
using namespace cv;


int main(int argc, char** argv )
{
    Mat src, grayscale, edges, cdst;
    src = imread(INFILE, IMREAD_COLOR);
    cvtColor(src, grayscale, COLOR_BGR2GRAY);
    int X = src.cols, Y = src.rows;
    
    blur(grayscale, edges, Size(3,3));
    Canny(edges, edges, LT, HT, 3);

    cvtColor(edges, cdst, COLOR_GRAY2BGR);

    vector<Vec2f> lines;
    HoughLines(edges, lines, 1, CV_PI/180, 50, 0 ,0);

    float rho, theta;
    Point p1, p2;
    double a, b, x0, y0;
    for(size_t i=0;i<lines.size();i++){
        rho = lines[i][0];
        theta = lines[i][1];
        a = cos(theta);
        b = sin(theta);
        x0 = a*rho;
        y0 = a*rho;
        p1.x = cvRound(x0 + X*(-b));
        p1.y = cvRound(y0 + Y*(a));
        p2.x = cvRound(x0 - X*(-b));
        p2.y = cvRound(y0 - Y*(a));
        line(cdst, p1, p2, Scalar(0, 0, 255), 3, LINE_AA);
    }

    imwrite("./gray.png", grayscale);
    imwrite("./edges.png", edges);
    imwrite("./lines.png", cdst);

    waitKey();
    
}