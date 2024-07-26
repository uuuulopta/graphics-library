#include <vector>
#include <iostream>
#include <fstream>
#include <string>
#include "windows.h"
#include "../headers/Canvas.h"
using namespace std;

#define WIDTH 960
#define HEIGHT 720

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

Point3d getMostLeftPoint(vector<Point3d> points)
{
    Point3d least = points[0];
    for (Point3d p : points)
    {
        if (p.x < least.x)
            least = p;
    }
    return least;
}

Point3d project_2d_3d(Point3d p) {
    return Point3d(p.x/p.z,p.y/p.z,z);
}

Point3d  normalized_to_screen(Point3d p)
{
    float x = ((p.x + 1)/2)*WIDTH;
    float y = ((p.y + 1)/2)*HEIGHT;
    return Point3d(x,y,p.z);
}

Point3d project(Point3d p){
   return normalized_to_screen(project_2d_3d(p));
}

int main(int argv, char **args)
{
    Canvas c = Canvas(WIDTH, HEIGHT);
    c.SDL_Init("prozor");
    c.Render_SDL();
    SDL_Event event;
    float dt;
    // Cube points NORMALIZED
    vector<Point3d> points =
     {
        Point3d(-0.5, -0.5, 1), Point3d(0, -0.5, 1), Point3d(0, 0, 1), Point3d(-0.5, 0, 1),
        Point3d(-0.5, -0.5, 1.5), Point3d(0, -0.5, 1.5), Point3d(0, 0, 1.5), Point3d(-0.5, 0, 1.5)
        };
    Point3d centroid = Point3d(0, 0, 0);

    // Calculate centroid of the cube (the cube will rotate around it's center)
    for (Point3d &p : points)
    {
        cout << (p.x) << " " << (p.y) << " " << p.z << endl;
        centroid.x += p.x;
        centroid.y += p.y;
        centroid.z += p.z;
    }
    centroid.x /= 8;
    centroid.y /= 8;
    centroid.z /= 8;

    float angle = 0.023;
    //Animate
    while (1)
    {
        c.clear();
        SDL_PollEvent(&event);
        int counter = 0;
        for (Point3d &p : points)
        {
            p.rotate(x,centroid,angle);
            p.rotate(y,centroid,angle);
            p.rotate(z,centroid,angle);
            // p.z-=0.01;
            
            Point3d normalized = normalized_to_screen(project_2d_3d(p));
            c.draw_circle(normalized.x, normalized.y, 5, 1, GREEN, GREEN);
        }
        // Connect points with lines
        for (int i = 0; i < 4; i++)
        {
            
            c.draw_line3d(project(points[i]), project(points[(i + 1) % 4]), RED);
            c.draw_line3d(project(points[i + 4]), project(points[((i + 1) % 4) + 4]), RED);
            c.draw_line3d(project(points[i]), project(points[i + 4]), RED);
        }
        c.Render_SDL();
        SDL_Delay(30);
    }
}
