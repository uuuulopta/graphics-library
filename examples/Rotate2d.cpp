#include "../headers/Example.h"
#include "../headers/Utils.h"
#include "square.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_keyboard.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_scancode.h>
#include <SDL2/SDL_stdinc.h>
#include <math.h>
#include <vector>
using namespace std;

void rotate2d::main() {
  bool quit = false;

  while (!quit) {
    Utils::pollEventQuit();
    Canvas c = Canvas(WIDTH, HEIGHT);
    c.SDL_Init(renderer);
    c.Render_SDL();
    vector<v2> xAxis = vector<v2>();
    vector<v2> yAxis = vector<v2>();
    const float scale = 10;
    for (int i = -scale; i <= scale; i++) {
      xAxis.push_back(v2(i, 0));
      yAxis.push_back(v2(0, i));
    }
    Utils::drawCordSystem2d(xAxis, yAxis, c, scale, BLACK);

    v2 tacka1 = v2(3, 0);
    c.draw_circle(Utils::normalize_scale(tacka1, scale), 2, 1, BLUE, BLUE);
    v2 centar = v2(0, 0);
    tacka1.rotate2dPoint(centar, M_PI_4);

    v2 t = Utils::normalize_scale(tacka1, scale);
    v2 t2 = Utils::normalize_scale(centar, scale);

    c.draw_circle(t, 2, 1, BLUE, BLUE);
    c.draw_circle(t2, 2, 1, GREEN, GREEN);
    array<v2, 2> tacke = {tacka1, centar};
    auto fontSmall = TTF_OpenFont("assets/DejaVuSans.ttf", 11);

    runBeforeRender(quit);
    c.Render_SDL();
    for (v2 v : tacke) {
      v2 cords = Utils::normalize_scale(v, scale);
      cout << cords.x << " " << cords.y << "\n";
      const string write =
          to_string(v.x).substr(0, 4) + ", " + to_string(v.y).substr(0, 4);
      c.renderText(fontSmall, write, cords.x, cords.y, BLACK);
    }
    SDL_RenderPresent(renderer);
    SDL_Event event;
    SDL_WaitEvent(&event);
    c.clear();
  }
}
