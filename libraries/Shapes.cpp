#include "../headers/Shapes.h"
#include "../headers/Utils.h"
#include <cmath>
#include <iostream>
#include <stdexcept>
#include <vector>
using namespace std;

void throwErr(const char *text) { cerr << text << endl; }
rgb colour;



v2::v2(){}
v2::v2(float x, float y){
  this->x = x;
  this->y = y;
}

v2 v3::tov2(){
  return v2{x,y};
}
void v2::shear(float xShear,float yShear){
  x += yShear * y;
  y += xShear * x;
}

v3::v3() {}
v3::v3(v2 p, float z) : v3(p.x, p.y, z) {};
v3::v3(float x, float y, float z) {
  this->x = x;
  this->y = y;
  this->z = z;
}

void v3::translate(float x, float y, float z) {
  this->x += x;
  this->y += y;
  this->z += z;
}

// 2d Rotation around a point
void v3::rotate2dPoint(v2 point, float angle) {
  double cosAngle = cos(angle);
  double sinAngle = sin(angle);
  this->x = (float)(cosAngle * (this->x - point.x) -
                    sinAngle * (this->y - point.y) + point.x);
  this->y = (float)(sinAngle * (this->x - point.x) +
                    cosAngle * (this->y - point.y) + point.y);
}


/*  If axis == x: s1 = Sy s2= Sz
 *
 *  If axis == y: s1 = Sx s2= Sz
 *
 *  If axis == z: s1 = Sx s2= Sy
 */
void v3::shear(Axis3d axis,float s1, float s2){
  if(axis == 'x'){
    y += s1 * x;
    z += s2 * x;
  }
  if(axis == 'y'){
    x += s1 * y;
    z += s2 * y;
  }
  if(axis == 'z'){
    x += s1 * z;
    x += s2 * z;
    
  }

}

// Rotate a 3d point
void v3::rotate(Axis3d axis, v3 origin, float angle) {
  x -= origin.x;
  y -= origin.y;
  z -= origin.z;
  float sinangle = sin(angle);
  float cosangle = cos(angle);
  if (axis == 'x') {
    float yn = y * cosangle - z * sinangle;
    float zn = y * sinangle + z * cosangle;
    y = yn;
    z = zn;
  } else if (axis == 'y') {
    float xn = x * cosangle + z * sinangle;
    float zn = z * cosangle - x * sinangle;
    x = xn;
    z = zn;

  } else if (axis == 'z') {
    float xn = x * cosangle - y * sinangle;
    float yn = x * sinangle + y * cosangle;
    x = xn;
    y = yn;
  }
  x += origin.x;
  y += origin.y;
  z += origin.z;
}

double m, b;
Line::Line() {};
Line::Line(v2 p1, v2 p2, rgb colour) : Line(v3(p1), v3(p2), colour) {};
Line::Line(v3 p1, v3 p2, rgb colour) : p1(p1), p2(p2) {
  this->colour = colour;
  calculate();
}
void Line::calculate() {

  height = abs(p2.y - p1.y);
  if (p1.y > p2.y) {
    topY = p1.y;
    bottomY = p2.y;
  } else {
    topY = p2.y;
    bottomY = p1.y;
  }
  if (p1.x > p2.x) {
    rightEdge = p1.x;
    leftEdge = p2.x;
  } else {
    rightEdge = p2.x;
    leftEdge = p1.x;
  }
  if (p1.z > p2.z) {
    deepZ = p1.z;
    shallowZ = p2.z;
  } else {
    deepZ = p2.z;
    shallowZ = p1.z;
  }

  if (p1.x != p2.x) {
    m = ((float)p2.y - p1.y) / ((float)p2.x - p1.x);
    b = p1.y - m * p1.x;
  }
}
// Returns integer x from a given y position on a line.
float Line::xFromY(float y) {
  // if division by 0
  // y = mx + b => y = b
  if (m == 0) {
    return b;
  }
  // if no slope
  if (p1.x == p2.x) {
    return p1.x;
  }
  float calc = (y - b) / m;

  return calc;
}
// Checks whether a given point is right of the line
int Line::isPointRight(float x, float y) {
  // if line is vertical
  if (p1.x == p2.x) {
    if (x == p1.x)
      return -1;
    if (x < p1.x)
      return 0;
    if (x > p1.x)
      return 1;
  };
  // if line is horizontal
  if (m == 0) {
    if (x > rightEdge)
      return 1;
    if (x < leftEdge)
      return 0;
    if (y == p1.y && x >= leftEdge && x <= rightEdge)
      return -1;
    else {
      cout << ("Error : The line is neither left nor right");
      throw logic_error("The line is neither left nor right");
    }
  }
  float res = y - m * x - b;

  if (res == 0)
    return -1;
  if (m > 0) {
    if (res < 0)
      return 1;
    if (res > 0)
      return 0;
  } else {
    if (res < 0)
      return 0;
    if (res > 0)
      return 1;
  }
  throw logic_error("Something went wrong...");
}
void Line::translate(float x, float y, float z) {
  p1.x += x;
  p2.x += x;
  p1.y += y;
  p2.y += y;
  p1.z += z;
  p2.z += z;
  topY += y;
  bottomY += y;
  leftEdge += x;
  rightEdge += x;
  b = p1.y - m * p1.x;
  // y = mx + b, b =  y
}
void Line::scale(float scaleX, float scaleY) {
  p1.x = p1.x * scaleX;
  p2.x = p2.x * scaleX;
  p1.y = p1.y * scaleY;
  p2.y = p2.y * scaleY;
  calculate();
}

Triangle::Triangle(Line a, Line b, Line c, rgb colour) {
  this->a = a;
  this->b = b;
  this->c = c;
  vector<v3> points = {a.p1, a.p2, b.p1, b.p2, c.p1, c.p2};
  vector<v3> unqiue = Utils::getUniqueV3(points);
  this->p1 = unqiue[0];
  this->p2 = unqiue[1];
  this->p3 = unqiue[2];
  setCenter();
}

void Triangle::createLinesFromPoints(rgb colour) {
  this->a = Line(this->p1, this->p2, colour);
  this->b = Line(this->p3, this->p2, colour);
  this->c = Line(this->p1, this->p3, colour);
  setCenter();
}

Triangle::Triangle(v3 a, v3 b, v3 c, rgb colour) {
  this->p1 = a;
  this->p2 = b;
  this->p3 = c;
  createLinesFromPoints(colour);
  setCenter();
}

void Triangle::setCenter() {
  this->centerX = (p1.x + p2.x + p3.x) / 3;
  this->centerY = (p1.y + p2.y + p3.y) / 3;
  this->centerZ = (p1.z + p2.z + p3.z) / 3;
}

// Returns a vector of lines where the first is the longest.
vector<Line> Triangle::getLongestEdge() const {
  vector<Line> lines = {a, b, c};
  for (int i = 0; i < 3; i++) {
    if (lines[i].height > lines[0].height)
      swap(lines[i], lines[0]);
  }
  return lines;
}
void Triangle::translate(float x, float y, float z) {
  p1.translate(x, y, z);
  p2.translate(x, y, z);
  p3.translate(x, y, z);
  createLinesFromPoints(a.colour);
  setCenter();
}

vector<v3> Triangle::getPoints() { return vector<v3>{p1, p2, p3}; }
v3 Triangle::getCenter() {
  setCenter();
  return v3{centerX, centerY, centerZ};
}

void Triangle::rotateAroundPoint3d(Axis3d axis, v3 origin, float angle) {
  this->p1.rotate(axis, origin, angle);
  this->p2.rotate(axis, origin, angle);
  this->p3.rotate(axis, origin, angle);
  createLinesFromPoints(a.colour);
  setCenter();
}

void Triangle::rotateAroundCenter2d(double angle) {
  v2 center = v2{centerX, centerY};
  this->p1.rotate2dPoint(center, angle);
  this->p2.rotate2dPoint(center, angle);
  this->p3.rotate2dPoint(center, angle);
  createLinesFromPoints(a.colour);
  setCenter();
}
void Triangle::scale(float scaleX, float scaleY) {
  a.scale(scaleX, scaleY);
  b.scale(scaleX, scaleY);
  c.scale(scaleX, scaleY);
}

Circle::Circle(float xc, float yc, float r, rgb colour) {
  this->xc = xc;
  this->yc = yc;
  this->r = r;
  this->colour = colour;
}
