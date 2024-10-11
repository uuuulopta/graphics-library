#pragma once
#include "Shapes.h"
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_ttf.h>
#include <string>

class Utils {
public:
  static v3 getMostLeftPoint(vector<v3> points);

  static v3 project_2d_3d(v3 p);

  /*
  -1 ..x... 1  => 0 .x+1.. 1 =>

  */
  static v3 normalized_to_screen(v3 p);

  static v3 project(v3 p);
  static vector<v3> getUniqueV3(vector<v3> vectors);
  static void renderText(SDL_Renderer *renderer, TTF_Font *font,
                         const string &text, int x, int y, SDL_Color color);
};
