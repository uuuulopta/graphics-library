#pragma once
#include <cmath>
#include <cstdint>
#include <iostream>
#include <ostream>
#include <vector>

using namespace std;

struct rgb {
  uint8_t r;
  uint8_t g;
  uint8_t b;
};

struct v2 {
  float x, y;
  v2(float x, float y);
  v2();

  v2 operator-(const float n) const { return v2(x - n, y - n); }
  v2 operator+(const float n) const { return v2(x + n, y + n); }
  v2 operator/(const float n) const { return v2(x / n, y / n); }
  v2 operator*(const float n) const { return v2(x * n, y * n); }
  v2 negate() {
    x = -x;
    y = -y;
    return *this;
  }
  friend ostream &operator<<(ostream &os, const v2 &v3) {
    std::cout << v3.x << " " << v3.y;
    return os;
  }
  void rotate2dPoint(v2 point, float angle) {
    double cosAngle = cos(angle);
    double sinAngle = sin(angle);
    float newX = (float)(cosAngle * (this->x - point.x) -
                         sinAngle * (this->y - point.y) + point.x);
    float newY = (float)(sinAngle * (this->x - point.x) +
                         cosAngle * (this->y - point.y) + point.y);
    this->x = newX;
    this->y = newY;
  }
  void shear(float xShear, float yShear);
};

enum Axis3d { x = 'x', y = 'y', z = 'z' };
struct v3 {

  float x, y, z;
  v3(float x, float y, float z);
  v3(v2 p, float z = 1);
  v3();
  void translate(float x, float y, float z);
  void rotate(Axis3d axis, v3 origin, float angle);
  void rotate2dPoint(v2 point, float angle);
  /*  If axis == x: s1 = Sy s2= Sz
   *
   *  If axis == y: s1 = Sx s2= Sz
   *
   *  If axis == z: s1 = Sx s2= Sy
   */
  void shear(Axis3d axis, float s1, float s2);
  v2 tov2();
  bool operator==(const v3 &v) { return (x == v.x && y == v.y && z == v.z); }
  v3 operator+(const v3 &v) const { return v3(x + v.x, y + v.y, z + v.z); }
  v3 operator-(const v3 &v) const { return v3(x - v.x, y - v.y, z - v.z); }
  v3 operator-(const float n) const { return v3(x - n, y - n, z - n); }
  v3 operator+(const float n) const { return v3(x + n, y + n, z + n); }
  v3 operator/(const float n) const { return v3(x / n, y / n, z / n); }
  v3 operator*(const float n) const { return v3(x * n, y * n, z * n); }
  friend ostream &operator<<(ostream &os, const v3 &v3) {
    std::cout << v3.x << " " << v3.y << " " << v3.z;
    return os;
  }
};

class Line {
  friend class Triangle;
  friend class Canvas;

private:
public:
  float topY, bottomY, rightEdge, leftEdge, deepZ, shallowZ, height;
  v3 p1;
  v3 p2;
  void translate(float x, float y, float z);
  rgb colour;
  double m, b;
  Line();
  Line(v2 p1, v2 p2, rgb colour);
  Line(v3 p1, v3 p2, rgb colour);
  // Returns integer x from a given y position on a line.
  float xFromY(float y);
  // Checks whether a given point is right of the line
  int isPointRight(float x, float y);

  void scale(float scaleX, float scaleY);
};

class Triangle {
  friend class Canvas;

private:
  Line a;
  Line b;
  Line c;
  v3 p1;
  v3 p2;
  v3 p3;
  float centerX;
  float centerY;
  float centerZ;
  void setCenter();
  void createLinesFromPoints(rgb colour);

public:
  Triangle(Line a, Line b, Line c, rgb colour = {80, 80, 80});
  Triangle(v3 a, v3 b, v3 c, rgb colour = {80, 80, 80});

  // Returns a vector of lines where the first is the longest.
  vector<Line> getLongestEdge() const;
  void translate(float x, float y, float z);
  void rotateAroundCenter2d(double angle);
  void rotateAroundPoint3d(Axis3d axis, v3 origin, float angle);
  void scale(float scaleX, float scaleY);
  vector<v3> getPoints();
  v3 getCenter();
};

class Circle {
  friend class Canvas;

private:
  float xc, yc, r;
  rgb colour;

public:
  Circle(float xc, float yc, float r, rgb colour);
};
