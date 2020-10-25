#include <iostream>

#include <fstream>

#include <math.h>

#include <string>

#include <iomanip>      // std::setprecision

#include <sstream>

using namespace std;

struct Point {

  double x, y;
  Point() {
    x = y = 0;
  }
  Point(double a, double b) {
    x = a, y = b;
  }
};

double area(double x1, double y1, double x2, double y2, double x3, double y3) {
  return abs((x1 * (y2 - y3) + x2 * (y3 - y1) + x3 * (y1 - y2)) / 2.0);
}

void printpoints(Point a, Point b, Point c, Point d) {
  cout << "Point a: " << a.x << ", " << a.y << "\n";
  cout << "Point b: " << b.x << ", " << b.y << "\n";
  cout << "Point c: " << c.x << ", " << c.y << "\n";
  cout << "Point d: " << d.x << ", " << d.y << "\n";
  cout << "Point a: " << (int)(a.x * 800) << ", " << (int)(a.y * 800) << "\n";
  cout << "Point b: " << (int)(b.x * 800) << ", " << (int)(b.y * 800) << "\n";
  cout << "Point c: " << (int)(c.x * 800) << ", " << (int)(c.y * 800) << "\n";
  cout << "Point d: " << (int)(d.x * 800) << ", " << (int)(d.y * 800) << "\n";
}

float side(float x1, float y1, float x2, float y2) {
  return sqrt(pow((y2 - y1), 2) + pow((x2 - x1), 2));
}

double distance(double x1, double y1, double x2, double y2) {
  return sqrt(pow((x1 - x2), 2) + pow((y1 - y2), 2));
}

bool inside(int x, int y) {
  if ((x < 800 && x >= 0) && (y < 800 && y >= 0)) {
    return true;
  } else {
    return false;
  }
}

bool inside_triangle(double x1, double y1, double x2, double y2, double x3, double y3, double testx, double testy) {

  double a = area(x1, y1, x2, y2, x3, y3);
  double a1 = area(x1, y1, x2, y2, testx, testy);
  double a2 = area(x2, y2, x3, y3, testx, testy);
  double a3 = area(x1, y1, x3, y3, testx, testy);
  return (a == a1 + a2 + a3);
}

void print_xy(int x1, int y1, int x2, int y2) {
  cout << "\n" << x1 << " " << y1 << " " << x2 << " " << y2 << "\n";
}

void draw_circle(int x1, int y1, int r, int a[800][800]) {
  int x, y, xmax, y2, y2_new, ty;
  xmax = (int)(r / sqrt(2)) + 1;
  y = r;
  y2 = y * y;
  ty = (2 * y) - 1;
  y2_new = y2;
  for (x = 0; x <= xmax; x++) {
    if ((y2 - y2_new) >= ty) {
      y2 -= ty;
      y -= 1;
      ty -= 2;
    }
    if (inside(x1 + x, y1 - y)) {
      a[x1 + x][y1 - y] = 1;
    }
    if (inside(x1 - x, y1 - y)) {
      a[x1 - x][y1 - y] = 1;
    }
    if (inside(x1 + x, y1 + y)) {
      a[x1 + x][y1 + y] = 1;
    }
    if (inside(x1 - x, y1 + y)) {
      a[x1 - x][y1 + y] = 1;
    }
    if (inside(x1 - y, y1 + x)) {
      a[x1 - y][y1 + x] = 1;
    }
    if (inside(x1 + y, y1 + x)) {
      a[x1 + y][y1 + x] = 1;
    }
    if (inside(x1 - y, y1 - x)) {
      a[x1 - y][y1 - x] = 1;
    }
    if (inside(x1 + y, y1 - x)) {
      a[x1 + y][y1 - x] = 1;
    }
    y2_new -= (2 * x) - 3;
  }
}

void draw_lines(int x1, int y1, int x2, int y2, int a[800][800], int c) {
  int ddy = y2 - y1;
  int ddx = x2 - x1;

  int g = abs(ddy);
  int q = abs(ddx);
  if (g > q) {
    if (ddy < 0) {
      int t = x1;
      x1 = x2;
      x2 = t;
      t = y1;
      y1 = y2;
      y2 = t;
    }
    int dx = x2 - x1;
    int dy = y2 - y1;
    int j = x1;
    int e = dy - dx;

    if (dx >= 0) {

      for (int i = y1; i < y2; i++) {
        if (inside(j, i)) {
          a[j][i] = c;
        }
        if (e >= 0) {
          j += 2;
          e -= dy;
        }
        i += 1;
        e += dx;
      }
    } else {
      for (int i = y1; i < y2; i++) {

        if (inside(j, i)) {
          a[j][i] = c;
        }
        if (e >= 0) {
          j -= 2;
          e -= dy;
        }
        i += 1;
        e -= dx;
      }
    }

  } else {
    if (ddx < 0) {
      int t = x1;
      x1 = x2;
      x2 = t;
      t = y1;
      y1 = y2;
      y2 = t;
    }
    int dx = x2 - x1;
    int dy = y2 - y1;
    int j = y1;
    int e = dy - dx;

    if (dy >= 0) {

      for (int i = x1; i < x2; i++) {
        if (inside(i, j)) {
          a[i][j] = c;
        }
        if (e >= 0) {
          j += 2;
          e -= dx;
        }
        i += 1;
        e += dy;
      }
    } else {
      for (int i = x1; i < x2; i++) {

        if (inside(i, j)) {
          a[i][j] = c;
        }
        if (e >= 0) {
          j -= 2;
          e -= dx;
        }
        i += 1;
        e -= dy;
      }
    }
  }
}

void draw_endlines(Point p1, Point p2, int a[800][800]) {
  double m = (p2.y - p1.y) / (p2.x - p1.x);

  Point e1 = Point(0, p1.y - m * p1.x);
  Point e2 = Point(1, m + e1.y);
  draw_lines((int)(e1.x * 800), (int)(e1.y * 800), (int)(e2.x * 800), (int)(e2.y * 800), a, 1);

}

void intersect(double a1, double b1, double c1, double a2, double b2, double c2, Point * p) {
  double determinant = a1 * b2 - a2 * b1;
  double x = (b2 * c1 - b1 * c2) / determinant;
  double y = (a1 * c2 - a2 * c1) / determinant;
  * p = Point(x, y);

}

void square(Point a, Point b, Point c, Point d, Point * sa, Point * sb, Point * sc, Point * sd, int arr[800][800]) {

  double slope1 = (b.y - a.y) / (b.x - a.x);
  double d1 = distance(a.x, a.y, b.x, b.y);
  double m12 = -1 / slope1;

  Point e1 = Point();
  Point e2 = Point();
  e1.x = c.x + d1 * sqrt(1 / (1 + m12 * m12));
  e1.y = c.y + m12 * d1 * sqrt(1 / (1 + m12 * m12));
  float tempd1 = distance(e1.x, e1.y, d.x, d.y);

  double finalad = (d.y - e1.y) / (d.x - e1.x);
  double finalbc = -1 / finalad;

  double alinea = -finalbc;
  double blinea = 1;
  double clinea = -1 * finalbc * a.x + a.y;

  double alineb = -finalbc;
  double blineb = 1;
  double clineb = -1 * finalbc * b.x + b.y;

  double alinec = -finalad;
  double blinec = 1;
  double clinec = -1 * finalad * c.x + c.y;

  double alined = -finalad;
  double blined = 1;
  double clined = -1 * finalad * d.x + d.y;

  Point q, w, e, r;

  intersect(alinea, blinea, clinea, alinec, blinec, clinec, & q);
  intersect(alinea, blinea, clinea, alined, blined, clined, & w);
  intersect(alineb, blineb, clineb, alinec, blinec, clinec, & e);
  intersect(alineb, blineb, clineb, alined, blined, clined, & r);

  * sa = q;
  * sb = w;
  * sc = e;
  * sd = r;

}

double square_area(Point a, Point b) {
  double d = distance(a.x, a.y, b.x, b.y);
  return d * d;
}

void part1() {
  srand(time(NULL));
  double t1x = double(rand()) / double((RAND_MAX));
  double t1y = double(rand()) / double((RAND_MAX));
  double t2x = double(rand()) / double((RAND_MAX));
  double t2y = double(rand()) / double((RAND_MAX));
  double t3x = double(rand()) / double((RAND_MAX));
  double t3y = double(rand()) / double((RAND_MAX));
  double t4x = double(rand()) / double((RAND_MAX));
  double t4y = double(rand()) / double((RAND_MAX));

  while (inside_triangle(t1x, t1y, t2x, t2y, t3x, t3y, t4x, t4y)) {
    t4x = double(rand()) / double((RAND_MAX));
    t4y = double(rand()) / double((RAND_MAX));
  }

  ofstream points("points.txt");
  points.precision(17);
  points << "(" << fixed << t1x << "," << t1y << ")";
  points << " , (" << t2x << "," << t2y << ")";
  points << " , (" << t3x << "," << t3y << ")";
  points << " , (" << t4x << "," << t4y << ")";

  points.close();
}

void part2() {
  int array[800][800];
  for (int y = 0; y < 800; y += 1) {
    for (int x = 0; x < 800; x += 1) {
      array[x][y] = 0;
    }
  }

  srand(time(NULL));



  Point q1 = Point(points[0], points[1]);
  Point q2 = Point(points[2], points[3]);
  Point q3 = Point(points[4], points[5]);
  Point q4 = Point(points[6], points[7]);

  array[(int)(q1.x * 800)][(int)(q1.y * 800)] = 2;
  array[(int)(q2.x * 800)][(int)(q2.y * 800)] = 2;
  array[(int)(q3.x * 800)][(int)(q3.y * 800)] = 2;
  array[(int)(q4.x * 800)][(int)(q4.y * 800)] = 2;

  Point sa1, sb1, sc1, sd1;
  Point sa2, sb2, sc2, sd2;
  Point sa3, sb3, sc3, sd3;
  Point sa4, sb4, sc4, sd4;
  Point sa5, sb5, sc5, sd5;
  Point sa6, sb6, sc6, sd6;
  square(q1, q2, q3, q4, & sa1, & sb1, & sc1, & sd1, array);
  square(q1, q2, q4, q3, & sa2, & sb2, & sc2, & sd2, array);
  square(q1, q3, q2, q4, & sa3, & sb3, & sc3, & sd3, array);
  square(q1, q3, q4, q2, & sa4, & sb4, & sc4, & sd4, array);
  square(q1, q4, q3, q2, & sa5, & sb5, & sc5, & sd5, array);
  square(q1, q4, q2, q3, & sa6, & sb6, & sc6, & sd6, array);
  double A1, A2, A3, A4, A5, A6;
  A1 = square_area(sa1, sb1);
  A2 = square_area(sa2, sb2);
  A3 = square_area(sa3, sb3);
  A4 = square_area(sa4, sb4);
  A5 = square_area(sa5, sb5);
  A6 = square_area(sa6, sb6);

  ofstream out("output.txt");
  out.precision(17);
  out << "(" << fixed << q1.x << "," << q1.y << ")";
  out << " , (" << q2.x << "," << q2.y << ")";
  out << " , (" << q3.x << "," << q3.y << ")";
  out << " , (" << q4.x << "," << q4.y << ")\n";

  out << "(" << fixed << sa1.x << "," << sa1.y << ")";
  out << " , (" << sb1.x << "," << sb1.y << ")";
  out << " , (" << sc1.x << "," << sc1.y << ")";
  out << " , (" << sd1.x << "," << sd1.y << ")";
  out << " Area = " << A1 << "\n";

  out << "(" << fixed << sa2.x << "," << sa2.y << ")";
  out << " , (" << sb2.x << "," << sb2.y << ")";
  out << " , (" << sc2.x << "," << sc2.y << ")";
  out << " , (" << sd2.x << "," << sd2.y << ")";
  out << " Area = " << A2 << "\n";

  out << "(" << fixed << sa3.x << "," << sa3.y << ")";
  out << " , (" << sb3.x << "," << sb3.y << ")";
  out << " , (" << sc3.x << "," << sc3.y << ")";
  out << " , (" << sd3.x << "," << sd3.y << ")";
  out << " Area = " << A3 << "\n";

  out << "(" << fixed << sa4.x << "," << sa4.y << ")";
  out << " , (" << sb4.x << "," << sb4.y << ")";
  out << " , (" << sc4.x << "," << sc4.y << ")";
  out << " , (" << sd4.x << "," << sd4.y << ")";
  out << " Area = " << A4 << "\n";

  out << "(" << fixed << sa5.x << "," << sa5.y << ")";
  out << " , (" << sb5.x << "," << sb5.y << ")";
  out << " , (" << sc5.x << "," << sc5.y << ")";
  out << " , (" << sd5.x << "," << sd5.y << ")";
  out << " Area = " << A5 << "\n";

  out << "(" << fixed << sa6.x << "," << sa6.y << ")";
  out << " , (" << sb6.x << "," << sb6.y << ")";
  out << " , (" << sc6.x << "," << sc6.y << ")";
  out << " , (" << sd6.x << "," << sd6.y << ")";
  out << " Area = " << A6 << "\n";

  out.close();

  Point draw1 = sa6;
  Point draw2 = sb6;
  Point draw3 = sc6;
  Point draw4 = sd6;

  double leastarea = min(A1, min(A2, min(A3, min(A4, min(A5, A6)))));
  // double leastarea = A1;
  if (leastarea == A1) {
    draw1 = sa1;
    draw2 = sb1;
    draw3 = sc1;
    draw4 = sd1;
  } else if (leastarea == A2) {
    draw1 = sa1;
    draw2 = sb1;
    draw3 = sc1;
    draw4 = sd1;
  } else if (leastarea == A2) {
    draw1 = sa2;
    draw2 = sb2;
    draw3 = sc2;
    draw4 = sd2;
  } else if (leastarea == A3) {
    draw1 = sa3;
    draw2 = sb3;
    draw3 = sc3;
    draw4 = sd3;
  } else if (leastarea == A4) {
    draw1 = sa4;
    draw2 = sb4;
    draw3 = sc4;
    draw4 = sd4;
  } else if (leastarea == A5) {
    draw1 = sa5;
    draw2 = sb5;
    draw3 = sc5;
    draw4 = sd5;
  } else if (leastarea == A6) {
    draw1 = sa6;
    draw2 = sb6;
    draw3 = sc6;
    draw4 = sd6;
  }

  array[(int)(draw1.x * 800)][(int)(draw1.y * 800)] = 1;
  array[(int)(draw2.x * 800)][(int)(draw2.y * 800)] = 1;
  array[(int)(draw3.x * 800)][(int)(draw3.y * 800)] = 1;
  array[(int)(draw4.x * 800)][(int)(draw4.y * 800)] = 1;

  draw_circle((int)(draw1.x * 800), (int)(draw1.y * 800), 2, array);
  draw_circle((int)(draw2.x * 800), (int)(draw2.y * 800), 2, array);
  draw_circle((int)(draw3.x * 800), (int)(draw3.y * 800), 2, array);
  draw_circle((int)(draw4.x * 800), (int)(draw4.y * 800), 2, array);
  draw_endlines(draw1, draw2, array);
  draw_endlines(draw1, draw3, array);
  draw_endlines(draw4, draw2, array);
  draw_endlines(draw4, draw3, array);

  ofstream square("output.ppm");
  square << "P3  800  800  1\n";

  for (int y = 0; y < 800; y += 1) {
    for (int x = 0; x < 800; x += 1) {
      if (array[x][y] == 0) {
        square << "0 0 0 ";
      } else if (array[x][y] == 1) {
        square << "1 1 1 ";
      } else if (array[x][y] == 2) {
        square << "0 1 0 ";
      } else {
        square << "1 0 0 ";
      }

    }
    square << "\n";
  }
  square.close();
}

int main() {
  // part1();
  part2();
}