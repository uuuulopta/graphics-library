#pragma once
#include <cmath>
#include <cstdint>
#include <vector>

using namespace std;

struct rgb {
  uint8_t r;
  uint8_t g;
  uint8_t b;
};

struct v2 {
  float x, y;
};

enum Axis3d { x = 'x', y = 'y', z = 'z' };
class v3 {
  friend class Line;
  friend class Canvas;

public:
  float x, y, z;
  v3(float x, float y, float z);
  v3(v2 p, float z = 1);
  v3();
  void translate(float x, float y, float z);
  void rotate(Axis3d axis, v3 origin, float angle);
  void rotate2dPoint(v2 point, float angle);
  bool operator==(const v3 &v) { return (x == v.x && y == v.y && z == v.z); }
};

class Line {
  friend class Triangle;
  friend class Canvas;

private:
public:
  float topY, bottomY, rightEdge, leftEdge, height;
  v3 p1;
  v3 p2;
  void calculate();
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
  void setCenter();
  void createLinesFromPoints(rgb colour);

public:
  Triangle(Line a, Line b, Line c, rgb colour = {80, 80, 80});
  Triangle(v3 a, v3 b, v3 c, rgb colour = {80, 80, 80});

  // Returns a vector of lines where the first is the longest.
  vector<Line> getLongestEdge();
  void translate(float x, float y, float z);
  void rotateAroundCenter2d(double angle);
  void rotateAroundPoint3d(Axis3d axis, v3 origin, float angle);
  void scale(float scaleX, float scaleY);
};

class Circle {
  friend class Canvas;

private:
  float xc, yc, r;
  rgb colour;

public:
  Circle(float xc, float yc, float r, rgb colour);
};
