#include <SDL2/SDL_events.h>
#include <SDL2/SDL_keyboard.h>
#include <SDL2/SDL_scancode.h>
#include <SDL2/SDL_timer.h>
#include <vector>
#include <iostream>
#include <fstream>
#include <string>
#include "../headers/Shapes.h"
#include "../headers/Canvas.h"
#include "../headers/objparser.h"
#include <chrono>
#include <thread>
using namespace std;

#define WIDTH 1920
#define HEIGHT 1080

#define RED {255,0,0}
#define GREEN { 0,255,0 }

#define BLACK   { 0,0,0 }
#define WHITE { 255, 255, 255 }

Point3d project_2d_3d(Point3d p)
{
    return Point3d(p.x / p.z, p.y / p.z, z);
}

/*
-1 ..x... 1  => 0 .x+1.. 1 =>

*/
Point3d normalized_to_screen(Point3d p)
{
    float x = ((p.x + 1) / 2) * WIDTH;
    float y = (((p.y + 1) / 2)) * HEIGHT;
    return Point3d(x, y, p.z);
}

Point3d project(Point3d p)
{
    return normalized_to_screen(project_2d_3d(p));
}

Line project_line(Line l)
{
    Point3d p1 = project(Point3d(l.x1, l.y1, l.z1));
    Point3d p2 = project(Point3d(l.x2, l.y2, l.z2));
    return Line(p1.x, p1.y, p2.x, p2.y, l.colour, p1.z, p2.z);
}

float xOrigin = 0;
float yOrigin = 0;
float zOrigin = 0;
void translateTriangles(vector<Triangle>& trigs, float x, float y, float z){
    xOrigin += x;
    yOrigin += y;
    zOrigin += z;
    for (Triangle & trig: trigs){
            vector<Line> edges = trig.getLongestEdge();
                for(Line& l: edges){
                    Point3d p1 = Point3d(l.x1, l.y1, l.z1);
                    Point3d p2 = Point3d(l.x2, l.y2, l.z2);

                    p1.x +=  x; 
                    p2.x +=  x; 
                    p1.y +=  y; 
                    p2.y +=  y; 
                    p1.z +=  z; 
                    p2.z +=  z; 
                    l = Line(p1.x, p1.y, p2.x, p2.y, RED, p1.z, p2.z);

                }
                trig = Triangle(edges[0],edges[1],edges[2]);
    }
}


void rotateTriangles(vector<Triangle>& trigs,Axis3d axis,float angle){
    for (Triangle & trig: trigs){
            vector<Line> edges = trig.getLongestEdge();
                for(Line& l: edges){
                    Point3d p1 = Point3d(l.x1, l.y1, l.z1);
                    Point3d p2 = Point3d(l.x2, l.y2, l.z2);
                    p1.rotate(axis,Point3d(0+xOrigin, 0+yOrigin, 1.5+zOrigin),angle); 
                    p2.rotate(axis,Point3d(0+xOrigin, 0+yOrigin, 1.5+zOrigin),angle); 
                    l = Line(p1.x, p1.y, p2.x, p2.y, RED, p1.z, p2.z);

                }
                trig = Triangle(edges[0],edges[1],edges[2]);
    }
}


int main(int argv, char **args)
{
    Canvas c = Canvas(WIDTH, HEIGHT);
    c.SDL_Init("prozor");
    c.Render_SDL();
    SDL_Event event;
    uint32_t last = SDL_GetTicks();
    uint32_t start = SDL_GetTicks();
    float dt;
    vector<Triangle> trigs = TparseObj("teapot.obj",c);
    cout << "got lines" << endl;
    vector<Line> lines_trig;
    
    translateTriangles(trigs,0,0,0); 
    while (1)
    {
        if(SDL_GetKeyboardState(nullptr)[SDL_SCANCODE_W]){
            translateTriangles(trigs,0,0,0.01); 
        }

        if(SDL_GetKeyboardState(nullptr)[SDL_SCANCODE_S]){
            translateTriangles(trigs,0,0,-0.01); 
        }
        if(SDL_GetKeyboardState(nullptr)[SDL_SCANCODE_E]) rotateTriangles(trigs,x,0.01 * 3.14);
        c.clear();
        start = SDL_GetTicks();
        dt = (start - last) / 1000.f;
        last = start;
        if (dt > 0.0)
        {
            
            for(Triangle& trig: trigs){
                vector<Line> edges = trig.getLongestEdge();
                for(Line& l: edges){
                    Point3d p1 = Point3d(l.x1, l.y1, l.z1);
                    Point3d p2 = Point3d(l.x2, l.y2, l.z2);
                    p1.rotate(y, Point3d(0+xOrigin, 0+yOrigin, 1.5+zOrigin), 0.25 * 3.14 * dt);
                    p2.rotate(y, Point3d(0+xOrigin, 0+yOrigin, 1.5+zOrigin), 0.25 * 3.14 * dt);
                    l.x1 = p1.x;
                    l.x2 = p2.x;
                    l.y1 = p1.y;
                    l.y2 = p2.y;
                    l = Line(p1.x, p1.y, p2.x, p2.y, RED, p1.z, p2.z);
                                   }
                
                trig = Triangle(edges[0],edges[1],edges[2]);
                Triangle trigProjected = Triangle(project_line( edges[0] ),project_line( edges[1] ),project_line( edges[2] ));
                c.draw_triangle(trigProjected,1,RED,0,BLUE);
            }

                   }

            SDL_PollEvent(&event);
            c.Render_SDL();
               }
}
