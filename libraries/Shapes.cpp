#include "../headers/Shapes.h"
#include <cmath>
#include <vector>
using namespace std;




void throwErr(const char* text){
    cerr <<  text << endl;
}
rgb colour;

Point3d::Point3d(float x, float y, float z){
    this->x = x;
    this->y = y;
    this->z = z;
}
// Rotate a 3d point
void Point3d::rotate(Axis3d axis, Point3d origin, float angle)
{
    x -= origin.x;
    y -= origin.y;
    z -= origin.z;
    float sinangle = sin(angle);
    float cosangle = cos(angle);
    if(axis == 'x'){
        x = x;
        float yn = y * cos(angle) - z * sin(angle);
        float zn = y * sin(angle) + z * cos(angle);
        y = yn;
        z = zn;
    }
    else if(axis == 'y'){
        float xn = x * cos(angle) + z * sin(angle);
        float zn = z * cos(angle) - x * sin(angle);
        x = xn;
        z = zn;

    }
    else if(axis == 'z'){
        z = z;
        float xn = x * cosangle - y * sinangle;
        float yn = x * sinangle + y * cosangle;
        x = xn;
        y = yn;
    }
    x += origin.x;
    y += origin.y;
    z += origin.z;
}

double m,b;
Line::Line(){}
Line::Line(float x1,float y1, float x2, float y2,rgb colour,float z1, float z2){
    this->colour = colour;
    this->x1 = x1;
    this->y1 = y1;
    this->x2 = x2;
    this->y2 = y2;
    this->z1 = z1;
    this->z2 = z2;
    calculate();
}
void Line::calculate(){

    height = abs(y2-y1);
    if(y1 > y2){
        topY = y1;
        bottomY = y2;
    }
    else{
        topY = y2;
        bottomY = y1; 
    }
    if(x1 > x2){
        rightEdge = x1;
        leftEdge = x2;
    }
    else{
        rightEdge = x2;
        leftEdge = x1;
    }
    if(x1 != x2) {
        m = ((double)y2-y1)/((double)x2-x1);
        b = y1 - m * x1;
    }

}
//Returns integer x from a given y position on a line.
float Line::xFromY(float y){
    // if division by 0
   // y = mx + b => y = b
    if(m == 0){
        return b;   
    }
    // if no slope 
    if(x1 == x2){
        return x1;
    }
    float calc = (y - b) / m;
    
    
    return   calc;
}
//Checks whether a given point is right of the line
int Line::isPointRight(float x, float y){
    // if line is vertical
    if(x1 == x2) {
        if(x == x1) return -1;
        if(x < x1) return 0;
        if(x > x1) return 1;
    };
    // if line is horizontal
    if(m == 0){
        if(x > rightEdge) return 1;
        if(x < leftEdge) return 0;
        if(y == y1 && x >= leftEdge && x <= rightEdge) return -1;
        else{
            cout << ("Error : The line is neither left nor right");
            throw logic_error("The line is neither left nor right");
        }
    }
    float res = y - m * x - b;

    if (res == 0 ) return -1;
    if(m > 0){
        if(res < 0) return 1;
        if(res > 0) return 0;
    }
    else{
        if(res < 0) return 0;
        if(res > 0) return 1;
    }

}  
void Line::translate(float x, float y){
    
    x1 += x;
    x2 += x;
    y1 += y;
    y2 += y;
    topY += y;
    bottomY += y;
    leftEdge += x;
    rightEdge += x;
    b = y1 - m * x1;
    // y = mx + b, b =  y

}
void Line::scale(float scaleX, float scaleY){
    x1 = x1*scaleX;
    x2 = x2*scaleX; 
    y1 = y1*scaleY;
    y2 = y2*scaleY; 
    calculate();
}



Triangle::Triangle(Line a, Line b, Line c){
    this->a = a;
    this->b = b;
    this->c = c;
    setCenter();
}



void Triangle::setCenter(){
//get the unique x and y values from lines
float x1 = a.x1;
float x2 = a.x2;
float x3;
// TODO error here
if(x1 != b.x1 && x2 != b.x1)  x3 = b.x1;
else if(x1 != b.x2 && x2 != b.x2)  x3 = b.x2;
else throwErr("x3 is undefined"); 
float y1 = a.y1;
float y2 = a.y2;
float y3;
if(y1 != b.y1 && y2 != b.y1)  y3 = b.y1;
else if(y1 != b.y2 && y2 != b.y2)  y3 = b.y2;
else{
    /* If we are here that means that there is a horizontal line between two points
        Logic Example to figure out the third point:
        lineA [ ((20,10) (10,5))]
        lineB [ ((10,5) (30,5))]
        lineC [ ((30,5) (20,10))]
        The more occuring y is the third point.
    */
    float y1Counter = 0;
    float y2Counter = 0;
    if (y1 == b.y1) y1Counter++;
    else y2Counter++;
    if (y1 == b.y2) y1Counter++;
    else y2Counter++; 
    if (y1 == c.y1) y1Counter++;
    else y2Counter++;
    if (y1 == c.y2) y1Counter++;
    else y2Counter++;
    if(y1Counter == y2Counter) throwErr("y3 is undefined");
    if(y1Counter > y2Counter) y3 = y1;
    else y3 = y2;   
    }
    this->centerX = (x1+x2+x3)/3;
    this->centerY = (y1+y2+y3)/3;

}

//Returns a vector of lines where the first is the longest.
vector<Line> Triangle::getLongestEdge() {
    vector<Line> lines = {a,b,c};
    for(int i = 0; i < 3; i++){
        if(lines[i].height > lines[0].height) swap(lines[i],lines[0]);
    }
    return lines;
}
void Triangle::translate(int x, int y){
    a.translate(x,y);
    b.translate(x,y);
    c.translate(x,y);
}

void Triangle::rotate(double deg){


//p'x = cos(theta) * (px-ox) - sin(theta) * (py-oy) + ox
double cosdeg = cos(deg);
double sindeg = sin(deg);


float ax1n = (float) ( cosdeg * (a.x1 - centerX) - sindeg * (a.y1 - centerY) + centerX );
float ax2n =(float) (  cosdeg * (a.x2 - centerX) - sindeg * (a.y2 - centerY) + centerX );
float bx1n =(float) (  cosdeg * (b.x1 - centerX) - sindeg * (b.y1 - centerY) + centerX );
float bx2n = (float) ( cosdeg * (b.x2 - centerX) - sindeg * (b.y2 - centerY) + centerX );
float cx1n =(float) (  cosdeg * (c.x1 - centerX) - sindeg * (c.y1 - centerY) + centerX );
float cx2n = (float) ( cosdeg * (c.x2 - centerX) - sindeg * (c.y2 - centerY) + centerX );
//  p'y = sin(thetc) * (px-ox) + cos(thetc) * (py-oy) + oy
float ay1n = (float) ( sindeg * (a.x1 - centerX) + cosdeg * (a.y1 - centerY) + centerY );
float ay2n = (float) ( sindeg * (a.x2 - centerX) + cosdeg * (a.y2 - centerY) + centerY );
float by1n = (float) ( sindeg * (b.x1 - centerX) + cosdeg * (b.y1 - centerY) + centerY );
float by2n = (float) ( sindeg * (b.x2 - centerX) + cosdeg * (b.y2 - centerY) + centerY );
float cy1n = (float) ( sindeg * (c.x1 - centerX) + cosdeg * (c.y1 - centerY) + centerY );
float cy2n = (float) ( sindeg * (c.x2 - centerX) + cosdeg * (c.y2 - centerY) + centerY );


 a.x1 = ax1n; 
 a.x2 = ax2n;
 b.x1 = bx1n; 
 b.x2 = bx2n; 
 c.x1 = cx1n;
 c.x2 = cx2n;
 a.y1 = ay1n; 
 a.y2 = ay2n; 
 b.y1 = by1n; 
 b.y2 = by2n; 
 c.y1 = cy1n; 
 c.y2 = cy2n; 

// if(a.x1 < 0 || a.x2 < 0 || b.x1 < 0 ||
//   b.x2 < 0 || c.x1 < 0 ||  c.x2 < 0 ||
//   a.y1 < 0 || a.y2 < 0 || b.y1 < 0  ||
//   b.y2 < 0 || c.y1 < 0 || c.y2 < 0){
//     cout << "hehe" << endl;
//   }

a.calculate();   
b.calculate();   
c.calculate();   

    }
void Triangle::scale(float scaleX,float scaleY){
    a.scale(scaleY,scaleY);
    b.scale(scaleY,scaleY);
    c.scale(scaleY,scaleY);

}


Circle::Circle(float xc,float yc ,float r,rgb colour){
    this->xc = xc;
    this->yc = yc;
    this->r = r;
    this->colour = colour;

}
