#include "../headers/Canvas.h"
#include "../headers/Example.h"
#include <SDL2/SDL_render.h>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
using namespace std;

// vscode formatted it like this :(
#define RED {255, 0, 0}
#define GREEN {0, 255, 0}
#define BLUE {0, 0, 255}
#define BLACK {0, 0, 0}
#define WHITE {255, 255, 255}

void CubeOrtho::main() {
  Canvas c = Canvas(WIDTH, HEIGHT);
  c.SDL_Init(renderer);
  c.Render_SDL();
  SDL_Event event;
  float dt;
  // Cube points
  vector<v3> points = {v3(400, 400, 400), v3(600, 400, 400), v3(600, 600, 400),
                       v3(400, 600, 400), v3(400, 400, 600), v3(600, 400, 600),
                       v3(600, 600, 600), v3(400, 600, 600)};
  v3 centroid = v3(0, 0, 0);

  // Calculate centroid of the cube (the cube will rotate around it's center)
  for (v3 &p : points) {
    cout << (p.x) << " " << (p.y) << " " << p.z << endl;
    centroid.x += p.x;
    centroid.y += p.y;
    centroid.z += p.z;
  }
  centroid.x /= 8;
  centroid.y /= 8;
  centroid.z /= 8;

  float angle = 0.023;
  bool quit = false;
  // Animate
  while (!quit) {
    c.clear();
    SDL_PollEvent(&event);
    int counter = 0;
    for (v3 &p : points) {
      p.z += 0.1;
      p.rotate(x, centroid, angle);
      p.rotate(y, centroid, angle);
      p.rotate(z, centroid, angle);
      c.draw_circle(p.x, p.y, 5, 1, GREEN, GREEN);
    }
    // Connect points with lines
    for (int i = 0; i < 4; i++) {
      c.draw_line3d(points[i], points[(i + 1) % 4], {255, 0, 0});
      c.draw_line3d(points[i + 4], points[((i + 1) % 4) + 4], {255, 0, 0});
      c.draw_line3d(points[i], points[i + 4], {255, 0, 0});
    }
    c.Render_SDL();
    runBeforeRender(quit);
    SDL_RenderPresent(renderer);
    SDL_Delay(30);
  }
}
