#pragma once
#include "Shapes.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <cstdint>
#include <string>

void throwErr(const char *text);
class Canvas {
  friend class Line;
  friend class Triangle;

private:
  int WIDTH;
  int HEIGHT;
  vector<float> zbuffer;
  rgb BG;
  string header;
  vector<rgb> canvas;
  SDL_Rect window_rect;
  SDL_Renderer *renderer = NULL;
  SDL_Texture *texture = NULL;
  void getMostLeftPoint(vector<v3> points);
  void drawAllCircleSides(float x, float y, Circle circle, int fill,
                          rgb fillColour);

public:
  Canvas(int w, int h, rgb bg = {255, 255, 255});
  int translatePoints(float x, float y);
  void setColour(rgb src, float x, float y);
  void setColour(rgb src, v3 p);
  void write(const char *filename);
  // Returns v2 containing {x,z} of drawn points
  vector<v3> draw_line(float x1, float y1, float x2, float y2,
                       int thickness = 1, rgb colour = {255, 255, 255},
                       v2 zInfo = {0, 0});
  void draw_line(Line line, int thickness = 1, rgb colour = {255, 255, 255});
  void draw_line(v2 p1,v2 p2,int thickness = 1, rgb colour = {255, 255, 255});
  // Returns v2 containing {x,z} of drawn points
  vector<v3> draw_line3d(v3 p1, v3 p2, rgb colour);
  vector<v3> draw_line3d_matrix(v3 p1, v3 p2, rgb colour,float fov,float aspectRatio,float zNear,float zFar);
  void draw_triangle3d(Triangle, rgb colour);
  void fill_triangle(const Triangle &trig, rgb colour);
  void fill_between_lines(Line line1, Line line2, rgb colour);
  Triangle draw_triangle(float x1, float y1, float x2, float y2, float x3,
                         float y3, int edgeThickness = 1,
                         rgb colour = {255, 255, 255}, int fill = 0,
                         rgb fillColour = {255, 255, 255});
  Triangle draw_triangle(Line a, Line b, Line c, int edgeThickness = 1,
                         rgb colour = {255, 255, 255}, int fill = 0,
                         rgb fillColour = {255, 255, 255});
  void draw_triangle(Triangle triangle, int edgeThickness = 1,
                     rgb colour = {255, 255, 255}, int fill = 0,
                     rgb fillColour = {255, 255, 255});
  void draw_circle(float xc, float yc, float r, int fill,
                   rgb colour = {255, 255, 255},
                   rgb fillColour = {255, 255, 255});
  void draw_circle(v2, float r, int fill,
                   rgb colour = {255, 255, 255},
                   rgb fillColour = {255, 255, 255});
  void translate(Triangle &triangle, float x, float y, int fill = 0,
                 rgb colour = {255, 255, 255});
  int getSize();
  void Render_SDL();
  void SDL_Init(const char *win_name);
  void SDL_Init(SDL_Renderer *renderer);
  void rgb_to_rgba(uint32_t *pixels);
  void clear();
  void setZbuffer(int x, int y, float val);
  void clearZbuffer();
  void renderText(TTF_Font *font, const string &text, int x, int y, SDL_Color color);

};
