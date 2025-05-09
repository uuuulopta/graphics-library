#pragma once
#include "Canvas.h"
#include "Shapes.h"
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_ttf.h>
#include <string>

class Utils {
public:
  static v3 getMostLeftPoint(vector<v3> points);

  static v3 project_2d_3d(v3 p);
  static v3 normalized_to_screen(v3 p);
  static v2 normalized_to_screen(v2 p);
  static Line project_line(Line l);
  static v2 normalize_scale(v2 v, float scale);
  static v3 normalize_scale(v3 v, float scale);
  static v3 project(v3 p);
  static v3 project(v3 p, v3 camera);
  static vector<v3> getUniqueV3(vector<v3> vectors);
  static v2 projectMatrix(const v3 &point, float fov, float aspectRatio,
                          float zNear, float zFar);
  static void renderText(SDL_Renderer *renderer, TTF_Font *font,
                         const string &text, int x, int y, SDL_Color color);
  static void drawCordSystem2d(vector<v2> xAxis, vector<v2> yAxis, Canvas &c,
                               float scale, rgb colour);
  static void pollEventQuit();
};
