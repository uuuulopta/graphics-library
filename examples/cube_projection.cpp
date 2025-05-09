#include <SDL2/SDL_keyboard.h>
#include <SDL2/SDL_keycode.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_scancode.h>
#include <SDL2/SDL_stdinc.h>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
// #include "windows.h"
#include "../headers/Canvas.h"
#include "../headers/Example.h"
using namespace std;

#include "../headers/Utils.h"
// vscode formatted it like this :(
#define RED {255, 0, 0}
#define GREEN {0, 255, 0}
#define BLUE {0, 0, 255}
#define BLACK {0, 0, 0}
#define WHITE {255, 255, 255}

void CubeProjection::main() {
  Canvas c = Canvas(WIDTH, HEIGHT);
  // c.SDL_Init("prozor");
  c.SDL_Init(renderer);
  c.Render_SDL();
  SDL_Event event;
  float dt;
  // Cube points NORMALIZED
  vector<v3> original = {
      v3(-0.5, -0.5, 1),   v3(0, -0.5, 1),   v3(0, 0, 1),   v3(-0.5, 0, 1),
      v3(-0.5, -0.5, 1.5), v3(0, -0.5, 1.5), v3(0, 0, 1.5), v3(-0.5, 0, 1.5)};

  for (v3 &p : original) {
    p.x += 0.25;
    p.y += 0.25;
    p.z += 0.2f;
  }
  vector<v3> points = original;
  float angle = 0.023;
  bool quit = false;

  float fov = 45.0f * (M_PI / 180.0f);
  float aspectRatio = 1;
  float zNear = 1.f;
  float zFar = 10.0f;

  // Animate
  while (!quit) {

    v3 centroid = v3(0, 0, 0);
    v3 camera = v3(0, 0.5, -1);
    // Calculate centroid of the cube (the cube will rotate around it's center)
    for (v3 &p : points) {
      // cout << (p.x) << " " << (p.y) << " " << p.z << endl;
      centroid.x += p.x;
      centroid.y += p.y;
      centroid.z += p.z;
    }
    centroid.x /= 8;
    centroid.y /= 8;
    centroid.z /= 8;
    c.clear();
    SDL_PollEvent(&event);
    bool bf = false;
    const Uint8 *state = SDL_GetKeyboardState(nullptr);
    vector<v3> pointsPre = points;
    for (v3 &p : points) {
      if (state[SDL_SCANCODE_X]) {
        p.rotate(x, centroid, angle);
      } else if (state[SDL_SCANCODE_Y]) {
        p.rotate(y, centroid, angle);
      } else if (state[SDL_SCANCODE_Z]) {
        p.rotate(z, centroid, angle);
      } else if (state[SDL_SCANCODE_UP]) {
        p.z += 0.01;
      } else if (state[SDL_SCANCODE_DOWN]) {
        p.z -= 0.01;
      } else if (state[SDL_SCANCODE_RIGHT]) {
        p.x += 0.01;
      } else if (state[SDL_SCANCODE_LEFT]) {
        p.x -= 0.01;
      } else if (state[SDL_SCANCODE_W]) {
        p.y += 0.01;
      } else if (state[SDL_SCANCODE_S]) {
        p.y -= 0.01;
      } else if (state[SDL_SCANCODE_R]) {
        points = original;
        bf = true;
        break;
      } else if (state[SDL_SCANCODE_G]) {
        p.shear(Axis3d::y, 0.01, 0.01);
      } else if (state[SDL_SCANCODE_H]) {
        p.shear(Axis3d::y, -0.01, -0.01);
      } else if (state[SDL_SCANCODE_COMMA]) {
        p = p * 1.01;
        cout << p << endl;
      } else if (state[SDL_SCANCODE_PERIOD]) {
        p = p / 1.01;
        cout << p << endl;
      }

      // if(p.z < 1.1f) {
      //   points = pointsPre;
      //   break;
      // }
      if (bf)
        break;
      // v3 normalized = Utils::normalized_to_screen(Utils::project_2d_3d(p));
      v3 normalized = Utils::projectMatrix(p, fov, aspectRatio, zNear, zFar);
      c.draw_circle(normalized.x, normalized.y, 5, 1, GREEN, GREEN);
    }
    if (bf)
      continue;
    // Connect points with lines
    for (int i = 0; i < 4; i++) {
      c.draw_line3d_matrix((points[i]), (points[(i + 1) % 4]), RED, fov,
                           aspectRatio, zNear, zFar);
      c.draw_line3d_matrix((points[i + 4]), (points[((i + 1) % 4) + 4]), RED,
                           fov, aspectRatio, zNear, zFar);
      c.draw_line3d_matrix((points[i]), (points[i + 4]), RED, fov, aspectRatio,
                           zNear, zFar);
    }
    c.Render_SDL();
    runBeforeRender(quit);
    SDL_RenderPresent(renderer);
  }
}
