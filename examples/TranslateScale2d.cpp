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

void TranslateScale2d::main() {
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

    v2 tacka1 = v2(3, 1);
    v2 tackaPomerena = tacka1;
    tackaPomerena.x += 4;

    v2 tacka2 = v2(2, 2);
    v2 tackaSkalirana = tacka2 * 2;

    v2 t1 = Utils::normalize_scale(tacka1, scale);
    v2 t1n = Utils::normalize_scale(tackaPomerena, scale);

    v2 t2 = Utils::normalize_scale(tacka2, scale);
    v2 t2n = Utils::normalize_scale(tackaSkalirana, scale);

    c.draw_circle(t1, 2, 1, BLUE, BLUE);
    c.draw_circle(t1n, 2, 1, BLUE, BLUE);

    c.draw_circle(t2, 2, 1, RED, RED);
    c.draw_circle(t2n, 2, 1, RED, RED);

    array<v2, 4> tacke = {tacka1, tackaPomerena, tacka2, tackaSkalirana};
    auto fontSmall = TTF_OpenFont("assets/DejaVuSans.ttf", 11);

    Utils::drawCordSystem2d(xAxis, yAxis, c, scale, BLACK);
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
