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
    Mat orig, grayscale, edges;
    orig = imread(INFILE, IMREAD_COLOR);
    cvtColor(orig, grayscale, COLOR_BGR2GRAY);
    
    blur(grayscale, edges, Size(3,3));
    Canny(edges, edges, LT, HT, 3);
    
    imwrite("./gray.jpg", grayscale);
    imwrite("./edges.jpg", edges);
    
}