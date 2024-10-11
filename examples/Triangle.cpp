#include <SDL2/SDL_render.h>
// #include "windows.h"
#include "../headers/Canvas.h"
#include "../headers/Example.h"
using namespace std;

#define RED {255, 0, 0}
#define GREEN {0, 255, 0}
#define BLUE {0, 0, 255}
#define BLACK {0, 0, 0}
#define WHITE {255, 255, 255}

void Trianglex::main() {
  Canvas canvas = Canvas(WIDTH, HEIGHT);
  v2 center = v2{WIDTH / 2.f, HEIGHT / 2.f};
  v2 pl = v2{center.x - 200, center.y - 200};
  v2 pr = v2{center.x + 200, center.y - 200};
  v2 pu = v2{center.x, center.y + 250};
  Line a = Line(pl, pr, GREEN);
  Line b = Line(pl, pu, GREEN);
  Line c = Line(pu, pr, GREEN);
  Triangle trig = Triangle(a, b, c);
  // c.SDL_Init("prozor");
  canvas.SDL_Init(renderer);
  canvas.Render_SDL();
  uint32_t last = SDL_GetTicks();
  uint32_t start = SDL_GetTicks();
  SDL_Event event;
  float dt;
  bool quit = false;
  while (!quit) {
    SDL_PollEvent(&event);
    start = SDL_GetTicks();
    dt = (start - last) / 1000.f;
    last = start;
    if (dt > 0.0) {
      canvas.clear();
      start = SDL_GetTicks();
      trig.rotateAroundCenter2d(0.73533 * dt);
      canvas.draw_triangle(trig, 1, RED, 1, BLUE);
      canvas.Render_SDL();
      runBeforeRender(quit);
      SDL_RenderPresent(renderer);
    }
  }
}
