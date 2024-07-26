#pragma once
#include <vector>
#include <iostream>
#include <fstream>
#include <string>
#include <cmath>

using namespace std;


struct rgb{
    uint8_t r;
    uint8_t g;
    uint8_t b;
};

enum Axis3d{x = 'x',y = 'y',z = 'z'};
class Point3d{
friend class Line;
friend class Canvas;
public:
    float x,y,z;
    Point3d(float x, float y, float z);
    void rotate(Axis3d axis, Point3d origin, float angle);
};

class Line{
friend class Triangle;
friend class Canvas;
private:
    void translate(float x, float y);
public:
    float x1,y1,x2,y2,z1,z2,topY,bottomY,rightEdge,leftEdge,height;
    void calculate();
    rgb colour;
    double m,b;
    Line();
    Line(float x1,float y1, float x2, float y2,rgb colour,float z1=1,float z2=1);
    //Returns integer x from a given y position on a line.
    float xFromY(float y);
    //Checks whether a given point is right of the line
    int isPointRight(float x, float y);  
    void scale(float scaleX,float scaleY);
};

class Triangle{
friend class Canvas;
private:
    Line a;
    Line b;
    Line c;
    float centerX;
    float centerY;
    void setCenter();
public: 
    Triangle(Line a, Line b, Line c);
     
    //Returns a vector of lines where the first is the longest.
    vector<Line> getLongestEdge();
    void translate(int x, int y);
    void rotate(double deg);
    void scale(float scaleX, float scaleY);
};

class Circle{
friend class Canvas;
private:
    float xc,yc,r;
    rgb colour;
public:
    Circle(float xc,float yc,float r,rgb colour);


};
