#include "../headers/Canvas.h"
#include "../headers/Example.h"
#include "../headers/Shapes.h"
#include "../headers/Utils.h"
#include "../headers/objparser.h"
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_keyboard.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_scancode.h>
#include <SDL2/SDL_stdinc.h>
#include <SDL2/SDL_timer.h>
#include <cfloat>
#include <iostream>
#include <string>
#include <vector>
using namespace std;

#define RED {255, 0, 0}
#define GREEN {0, 255, 0}
#define BLUE {0, 0, 255}
#define BLACK {0, 0, 0}
#define WHITE {255, 255, 255}

Line project_line(Line l) {
  v3 p1 = Utils::project(v3(l.p1.x, l.p1.y, l.p1.z));
  v3 p2 = Utils::project(v3(l.p2.x, l.p2.y, l.p2.z));
  return Line(p1, p2, l.colour);
}

void translateTriangles(vector<Triangle> &trigs, float x, float y, float z,
                        v3 &origin) {
  origin.translate(x, y, z);
  for (Triangle &trig : trigs) {
    trig.translate(x, y, z);
  }
}

void rotateTriangles(vector<Triangle> &trigs, Axis3d axis, float angle,
                     v3 &origin) {
  for (Triangle &trig : trigs)
    trig.rotateAroundPoint3d(axis, origin, angle);
}

Obj::Obj(std::string path_to_obj, int WIDTH, int HEIGHT, SDL_Renderer *renderer,
         TTF_Font *font)
    : Example(WIDTH, HEIGHT, renderer, font) {
  path = path_to_obj;
}

void Obj::main() {
  v3 origin = v3(0, 0, 0);
  Canvas c = Canvas(WIDTH, HEIGHT);
  // c.SDL_Init("prozor");
  c.SDL_Init(renderer);
  c.Render_SDL();
  SDL_Event event;
  uint32_t last = SDL_GetTicks();
  uint32_t start = SDL_GetTicks();
  float dt;
  // vector<Triangle> trigs = TparseObj(path, c);
  vector<Triangle> trigs = TparseObj(path, c);
  cout << "got lines " << endl;
  cout << std::to_string(WIDTH) << " " << std::to_string(HEIGHT) << endl;
  vector<Line> lines_trig;

  float zmin = FLT_MAX;
  float zmax = FLT_MIN;
  float xmax = FLT_MIN;
  float ymax = FLT_MIN;
  for (Triangle &trig : trigs) {
    for (Line l : trig.getLongestEdge()) {
      zmin = min(zmin, l.p1.z);
      zmin = min(zmin, l.p2.z);
      zmax = max(zmax, l.p1.z);
      zmax = max(zmax, l.p2.z);
      xmax = max(xmax, l.p1.x);
      xmax = max(xmax, l.p2.x);
      ymax = max(ymax, l.p1.y);
      ymax = max(ymax, l.p2.y);
    }
  }
  float zt = max(ymax, xmax);
  // translate so the object fits into the viewport
  translateTriangles(trigs, -0.0, -0.0, 2 + (zt * 2), origin);
  bool quit = false;
  while (!quit) {
    c.clear();
    start = SDL_GetTicks();
    dt = (start - last) / 1000.f;
    last = start;
    if (dt > 0.0) {
      const Uint8 *state = SDL_GetKeyboardState(nullptr);
      if (state[SDL_SCANCODE_UP]) {
        translateTriangles(trigs, 0.0, zt * 2 * 0.05, 0.0, origin);
      } else if (state[SDL_SCANCODE_DOWN]) {
        translateTriangles(trigs, 0.0, -zt * 2 * 0.05, 0.0, origin);
      } else if (state[SDL_SCANCODE_RIGHT]) {
        translateTriangles(trigs, zt * 2 * 0.05, 0.0, 0.0, origin);
      } else if (state[SDL_SCANCODE_LEFT]) {
        translateTriangles(trigs, -zt * 2 * 0.05, 0.0, 0.0, origin);
      } else if (state[SDL_SCANCODE_W]) {
        translateTriangles(trigs, 0.0, 0.0, zt * 2 * 0.05, origin);
      } else if (state[SDL_SCANCODE_S]) {
        translateTriangles(trigs, 0.0, 0.0, -zt * 2 * 0.05, origin);
      }
      rotateTriangles(trigs, Axis3d::y, 1.304533 * dt, origin);
      for (Triangle &trig : trigs) {
        vector<Line> edges = trig.getLongestEdge();
        trig = Triangle(edges[0], edges[1], edges[2]);
        Triangle trigProjected =
            Triangle(project_line(edges[0]), project_line(edges[1]),
                     project_line(edges[2]));
        c.draw_triangle(trigProjected, 1, rgb{80, 80, 80}, 1, rgb{80, 80, 80});
      }
    }

    SDL_PollEvent(&event);
    c.Render_SDL();
    runBeforeRender(quit);
    SDL_RenderPresent(renderer);
  }
}
