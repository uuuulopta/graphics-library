#include <SDL2/SDL_render.h>
#include <vector>
// #include "windows.h"
#include "../headers/Canvas.h"
#include "../headers/Example.h"
using namespace std;

#define RED {255, 0, 0}
#define GREEN {0, 255, 0}
#define BLUE {0, 0, 255}
#define BLACK {0, 0, 0}
#define WHITE {255, 255, 255}

void CubeDots::main() {
  Canvas c = Canvas(WIDTH, HEIGHT);
  Line a = Line(v2{100, 100}, v2{300, 300}, GREEN);
  Line b = Line(v2{300, 300}, v2{500, 100}, GREEN);
  Line d = Line(v2{500, 100}, v2{100, 100}, GREEN);
  Line a1 = Line(v2{120, 120}, v2{220, 220}, GREEN);
  Line b2 = Line(v2{220, 220}, v2{320, 120}, GREEN);
  Line d3 = Line(v2{320, 120}, v2{120, 120}, GREEN);
  Triangle trig = Triangle(a, b, d);
  Triangle trig2 = Triangle(a1, b2, d3);
  // c.setColour(BLUE,200,133);
  trig.translate(100, 0, 0);
  // c.draw_triangle(trig,1,BLUE);
  c.SDL_Init(renderer);
  c.Render_SDL();
  SDL_Event event;
  float dt;

  // (0, 0, 0, 0), (1, 0, 0, 0), (1, 1, 0, 0), (0, 1, 0, 0),
  // (0, 0, 1, 0), (1, 0, 1, 0), (1, 1, 1, 0), (0, 1, 1, 0),
  // (0, 0, 1, 1), (1, 0, 1, 1), (1, 1, 1, 1), (0, 1, 1, 1),
  // (0, 0, 0, 1), (1, 0, 0, 1), (1, 1, 0, 1), (0, 1, 0, 1
  float unit = 170.f;
  vector<v3> points = {v3(400, 400, 2), v3(500, 400, 2), v3(500, 500, 2),
                       v3(400, 500, 2), v3(400, 400, 3), v3(500, 400, 3),
                       v3(500, 500, 3), v3(400, 500, 3)};
  // for(int x = 1; x <= 2; x++){
  //     for(int y = 1; y<=2; y++){
  //         for(float z = 1; z<=2; z+=1){
  //             Point3d p = Point3d(unit*x,unit*y,z);
  //             points.push_back(p);
  //         }
  //     }
  // } //

  c.Render_SDL();
  // int counter = 0;
  // c.draw_line3d(points[0],points[1],RED);
  // c.draw_line3d(points[0],points[2],RED);
  // c.draw_line3d(points[4],points[5],RED);
  // c.draw_line3d(points[5],points[1],RED);
  // c.draw_line3d(points[4],points[6],RED);
  // c.draw_line3d(points[4],points[5],RED);
  // c.draw_line3d(points[5],points[7],RED);
  // c.draw_line3d(points[7],points[6],RED);
  // c.draw_line3d(points[7],points[3],RED);
  // c.draw_line3d(points[3],points[1],RED);
  // c.draw_line3d(points[2],points[6],RED);
  // Point3d rotpoint = getMostLeftPoint(points);
  v3 centroid = v3(0, 0, 0);

  for (v3 &p : points) {
    centroid.x += p.x;
    centroid.y += p.y;
    centroid.z += p.z;
  }
  centroid.x /= 8;
  centroid.y /= 8;
  centroid.z /= 8;
  // while(1)
  // {
  //     for(Point3d& p: points){
  //     // cout << (p.x/320)-1 << " " << (p.y/320)-1 << " " << p.z-1 <<" #" <<
  //     counter << endl;
  //    p.rotate(x,0.00198f,centroid.x,centroid.y,centroid.z);
  //     p.rotate(y,0.01198f,centroid.x,centroid.y,centroid.z);
  //     // cout << (p.x) << " " << (p.y) << " " << p.z << endl;
  //         // cout << (p.x) << " " << (p.y) << " " << p.z << endl;
  // c.draw_circle((p.x),(p.y),2,1,GREEN,GREEN);
  //         }

  c.Render_SDL();
  c.clear();
  uint32_t last = SDL_GetTicks();
  uint32_t start = SDL_GetTicks();
#define GRID_COUNT 8
#define GRID_PAD 0.25 / GRID_COUNT
#define GRID_SIZE ((GRID_COUNT - 1) * GRID_PAD)
#define Z_START 0.25
#define PI 3.14159265359
  bool quit = false;
  static float angle = 0;
  while (!quit) {
    c.clear();
    SDL_PollEvent(&event);
    start = SDL_GetTicks();
    dt = (start - last) / 1000.f;
    last = start;
    angle += 0.25 * PI * dt;
    for (int ix = 0; ix < GRID_COUNT; ++ix) {
      for (int iy = 0; iy < GRID_COUNT; ++iy) {
        for (int iz = 0; iz < GRID_COUNT; ++iz) {
          float x = ix * GRID_PAD - GRID_SIZE / 2;
          float y = iy * GRID_PAD - GRID_SIZE / 2;
          float z = Z_START + iz * GRID_PAD;

          float cx = 0.0;
          float cz = Z_START + GRID_SIZE / 2;

          float dx = x - cx;
          float dz = z - cz;

          float a = atan2f(dz, dx);
          float m = sqrtf(dx * dx + dz * dz);

          dx = cosf(a + angle) * m;
          dz = sinf(a + angle) * m;

          x = dx + cx;
          z = dz + cz;

          x /= z;
          y /= z;

          uint32_t r = ix * 255 / GRID_COUNT;
          uint32_t g = iy * 255 / GRID_COUNT;
          uint32_t b = iz * 255 / GRID_COUNT;
          c.draw_circle((x + 1) / 2 * WIDTH, (y + 1) / 2 * HEIGHT, 5, 1, GREEN,
                        GREEN);
        }
      }
    }
    c.Render_SDL();
    runBeforeRender(quit);
    SDL_RenderPresent(renderer);
    SDL_Delay(30);
  }
}
