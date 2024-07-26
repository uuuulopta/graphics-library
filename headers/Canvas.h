#pragma once
#include "Shapes.h"
#include <SDL2/SDL.h>

void throwErr(const char *text);
class Canvas
{
    friend class Line;
    friend class Triangle;
    
private:
    int WIDTH;
    int HEIGHT;
    rgb BG;
    string header;
    vector<rgb> canvas;
    SDL_Rect window_rect;
    SDL_Renderer *renderer = NULL;
    SDL_Texture *texture = NULL;
    void getMostLeftPoint(vector<Point3d> points);
    void drawAllCircleSides(float x, float y, Circle circle,int fill,rgb fillColour);

public:
    Canvas(int w, int h, rgb bg = {255, 255, 255});
    int translatePoints(float x, float y);
    void setColour(rgb src, float x, float y);
    void setColour(rgb src, Point3d p);
    void write(const char *filename);
    Line draw_line(float x1, float y1, float x2, float y2,int thickness = 1, rgb colour = {255,255,255});
    void draw_line(Line line, int thickness = 1,rgb colour = {255,255,255});
    void draw_line3d(Point3d p1, Point3d p2, rgb colour);
    void fill_between_lines(Line line1, Line line2, rgb colour);
    Triangle draw_triangle(float x1, float y1, float x2, float y2, float x3, float y3,int edgeThickness = 1, rgb colour = {255, 255, 255}, int fill = 0, rgb fillColour = {255,255,255});
    Triangle draw_triangle(Line a, Line b, Line c, int edgeThickness = 1, rgb colour = {255, 255, 255}, int fill = 0, rgb fillColour = {255,255,255});
    void draw_triangle(Triangle triangle, int edgeThickness = 1, rgb colour = {255, 255, 255}, int fill = 0, rgb fillColour = {255,255,255});
    void draw_circle(float xc, float yc, float r,int fill, rgb colour={255,255,255},rgb fillColour={255,255,255}); 
    void translate(Triangle &triangle, float x, float y, int fill = 0, rgb colour = {255, 255, 255});
    int getSize();
    void Render_SDL();
    void SDL_Init(const char *win_name);
    void rgb_to_rgba(uint32_t *pixels);
    void clear();
};
