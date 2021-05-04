// Anup Bagali
// Period 7
// 04/30/2021
#include <bits/stdc++.h>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#define INFILE "image.jpg"
#define HT 140

using namespace std;
using namespace cv;

const Scalar RED(0, 0, 255);
const Scalar BLUE(255, 0, 0);
const Scalar GREEN(0, 255, 0);
const Scalar PURPLE(255, 0, 255);
const Scalar YELLOW(0, 255, 255);

int main(int argc, char** argv )
{
    Mat src, grayscale, blurred, edges;
    src = imread(INFILE, IMREAD_COLOR);
    resize(src, src, Size(), .5, .5, INTER_LINEAR);
    cvtColor(src, grayscale, COLOR_BGR2GRAY);
    
    medianBlur(grayscale, grayscale, 5);
    Canny(grayscale, edges, HT/2, HT, 3);

    vector<Vec3f> circles;
    HoughCircles(grayscale, circles, HOUGH_GRADIENT, 1, grayscale.rows/30, HT, 40, 40, grayscale.rows/20);
              // src, dest, --,                  scale, minDist, Canny Thres, Votes, minRad, maxRad

    int p = 0;
    int n = 0;
    int d = 0;
    int q = 0;
    int sd = 0;
    int radius;
    Scalar color;
    for(size_t i=0;i<circles.size();i++){
        Vec3i c = circles[i];
        Point center = Point(c[0], c[1]);
        radius = c[2]; 
        if(radius<47){
            Vec3b g = grayscale.at<Vec3b>(c[0], c[1]);
            if(g[0] < 230){
                p++;
                color = RED;
            }else{
                d++;
                color = BLUE;
            }
        }else if(radius < 55){
            n++;
            color = PURPLE;
        }else if(radius<63){
            q++;
            color = GREEN;
        }else{
            sd++;
            color = YELLOW;
        }
        circle(src, center, radius, color, 3, LINE_AA);
    }
    double total = p + 10*d + 5*n + 25*q + 100*sd;
    cout << "Pennies: " << p << endl;
    cout << "Dimes: " << d << endl;
    cout << "Nickels: " << n << endl;
    cout << "Quarters: " << q << endl;
    cout << "Silver Dollars: " << sd << endl;
    cout << "Total Value: $" << total/100 << endl;

    ofstream out("results.txt");
    out << "Pennies: " << p << endl;
    out << "Dimes: " << d << endl;
    out << "Nickels: " << n << endl;
    out << "Quarters: " << q << endl;
    out << "Silver Dollars: " << sd << endl;
    out << "Total Value: $" << total/100 << endl;
    out.close();



    imwrite("./imagec.jpg", src);
}