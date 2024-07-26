#include <vector>
#include <iostream>
#include <fstream>
#include <string>
#include "windows.h"
#include "headers/Canvas.h"
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

int main(int argv, char **args)
{
    Canvas c = Canvas(500, 500);
    rgb lc = {0, 0, 255};
    Line a = Line(100, 100, 200, 200, GREEN);
    Line b = Line(200, 200, 300, 100, GREEN);
    Line d = Line(300, 100, 100, 100, GREEN);
    Line a1 = Line(120, 120, 220, 220, GREEN);
    Line b2 = Line(220, 220, 320, 120, GREEN);
    Line d3 = Line(320, 120, 120, 120, GREEN);
    Triangle trig = Triangle(a, b, d);
    Triangle trig2 = Triangle(a1, b2, d3);
    c.setColour(BLUE, 200, 133);
    c.SDL_Init("prozor");
    

    c.Render_SDL();
    uint32_t last = SDL_GetTicks();
    uint32_t start = SDL_GetTicks();
    SDL_Event event;
    float dt;
    while (1)
    {
        SDL_PollEvent(&event);
        start = SDL_GetTicks();
        dt = (start - last) / 1000.f;
        last = start;
        if (dt > 0.0)
        {
            c.clear();
            
            start = SDL_GetTicks();
            trig.rotate(0.73533 * dt);
            c.draw_triangle(trig, 1, RED,1,BLUE);
            c.setColour(GREEN, 200, 133);
            c.Render_SDL();
        }
    }
}
