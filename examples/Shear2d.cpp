#include "../headers/Example.h"
#include "../headers/Utils.h"
#include "square.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_keyboard.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_scancode.h>
#include <SDL2/SDL_stdinc.h>
#include <vector>
using namespace std;

void shear2d::main() {
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

    array<v2, 4> tacke = {v2(1, 2), v2(2, 2), v2(2, 1), v2(1, 1)};
    for (v2 t : tacke) {
      c.draw_circle(Utils::normalize_scale(t, scale), 2, 1, BLUE, BLUE);
    }
    tacke[0].shear(1, 0);
    tacke[1].shear(-1, 0);
    tacke[2].shear(0, 1);
    tacke[3].shear(0, -1);
    for (v2 t : tacke) {
      c.draw_circle(Utils::normalize_scale(t, scale), 2, 1, RED, RED);
    }

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
