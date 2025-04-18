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
#include <algorithm>
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
#define YELLOW {255, 255, 0}

float dotProduct(v3 a, v3 b) { return a.x * b.x + a.y * b.y + a.z * b.z; }
float magnitude(const v3 &v) {
  return sqrt(pow(v.x, 2) + pow(v.y, 2) + pow(v.z, 2));
}

v3 normalize(const v3 &v) {
  float mag = magnitude(v);
  return v3(v.x / mag, v.y / mag, v.z / mag);
}

v3 getNormal(Triangle t) {
  vector<v3> p = t.getPoints();
  v3 A = p[1] - p[0];
  v3 B = p[2] - p[0];
  return v3(A.y * B.z - A.z * B.y, A.z * B.x - A.x * B.z,
            A.x * B.y - A.y * B.x);
}
// https://sites.nova.edu/mjl/graphics/lighting/flat-and-smooth-shading/
float getFlatShading(Triangle &t, v3 lightPos) {
  v3 faceNormal = normalize(getNormal(t));
  v3 lightVector = normalize(lightPos - t.getCenter());
  float res = fmax(0.0f, dotProduct(faceNormal, lightVector)) /
              (magnitude(faceNormal) * magnitude(lightVector));
  if (res < 0)
    res = 0;
  else if (res > 1)
    res = 1;
  return res;
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

// Triangle getProjectedTriangle(const Triangle &t) {
//
//   vector<Line> edges = t.getLongestEdge();
//   Triangle trig = Triangle(edges[0], edges[1], edges[2]);
//   return Triangle(Utils::project_line(edges[0]),
//   Utils::project_line(edges[1]),
//                   Utils::project_line(edges[2]));
// }

void coutAll(vector<float> v) {
  for (int n : v) {
    cout << n << " ";
  }
}
Obj::Obj(std::string path_to_obj, int WIDTH, int HEIGHT, SDL_Renderer *renderer,
         TTF_Font *font)
    : Example(WIDTH, HEIGHT, renderer, font) {
  path = path_to_obj;
}

void Obj::main() {
  v3 origin = v3(0, 0, 0);
  Canvas c = Canvas(WIDTH, HEIGHT, {33, 33, 33});
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
  cout << "Projected values\n" << "----------------\n";
  coutAll(vector<float>{xmax, ymax});
  cout << endl;

  // translate so the object fits into the viewport
  translateTriangles(trigs, -0.0, 0, 2 + (zt * 2), origin);
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
      v3 lightPoint = v3(0, 1, zt + 1);
      for (Triangle &trig : trigs) {

        float lightEffect = getFlatShading(trig, lightPoint);

        rgb rgb = {static_cast<uint8_t>(255 * lightEffect),
                   static_cast<uint8_t>(255 * lightEffect),
                   static_cast<uint8_t>(255 * lightEffect)};
        c.draw_triangle3d(trig, rgb);
      }
      v3 lightPointProjected = Utils::project(lightPoint);
      c.draw_circle(lightPointProjected.x, lightPointProjected.y, 5, 1, YELLOW,
                    YELLOW);
      c.draw_line3d(origin, lightPoint, RED);
      c.clearZbuffer();
    }

    SDL_PollEvent(&event);
    c.Render_SDL();
    runBeforeRender(quit);
    SDL_RenderPresent(renderer);
  }
}
