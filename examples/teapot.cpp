#include <vector>
#include <iostream>
#include <fstream>
#include <string>
#include "windows.h"
#include "../headers/Canvas.h"
#include "../headers/objparser.h"
#include <chrono>
#include <thread>
using namespace std;

#define WIDTH 1920
#define HEIGHT 1080

// vscode formatted it like this :(
#define RED       \
    {             \
        255, 0, 0 \
    }
#define GREEN     \
    {             \
        0, 255, 0 \
    }
#define BLUE      \
    {             \
        0, 0, 255 \
    }
#define BLACK   \
    {           \
        0, 0, 0 \
    }
#define WHITE         \
    {                 \
        255, 255, 255 \
    }

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

int main(int argv, char **args)
{
    Canvas c = Canvas(WIDTH, HEIGHT);
    c.SDL_Init("prozor");
    c.Render_SDL();
    SDL_Event event;
    uint32_t last = SDL_GetTicks();
    uint32_t start = SDL_GetTicks();
    float dt;
    vector<Line> lines = parseObj("teapot.obj", c);
    cout << "got lines" << endl;
    for (Line &line : lines)
    {
        Point3d p1 = project(Point3d(line.x1, line.y1, line.z1));
        Point3d p2 = project(Point3d(line.x2, line.y2, line.z2));
        Line prj = Line(p1.x, p1.y, p2.x, p2.y, RED, p1.z, p2.z);
        c.draw_line(prj, 1, RED);
        SDL_PollEvent(&event);
    }
    c.Render_SDL();
    SDL_PollEvent(&event);
    vector<Line> lines_trig;
    while (1)
    {
        c.clear();
        start = SDL_GetTicks();
        dt = (start - last) / 1000.f;
        last = start;
        if (dt > 0.0)
        {
            for (Line &l : lines)
            {
                Point3d p1 = Point3d(l.x1, l.y1, l.z1);
                Point3d p2 = Point3d(l.x2, l.y2, l.z2);
                p1.rotate(y, Point3d(0, 0, 1.5), 0.25 * 3.14 * dt);
                p2.rotate(y, Point3d(0, 0, 1.5), 0.25 * 3.14 * dt);
                l.x1 = p1.x;
                l.x2 = p2.x;
                l.y1 = p1.y;
                l.y2 = p2.y;
                l = Line(p1.x, p1.y, p2.x, p2.y, RED, p1.z, p2.z);
                Line prj = project_line(l);
                c.draw_line(prj, 1, RED);
            }
            SDL_PollEvent(&event);
            c.Render_SDL();
        }
    }
}