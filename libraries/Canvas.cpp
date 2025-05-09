#include "../headers/Canvas.h"
#include "../headers/Utils.h"
#include <SDL2/SDL.h>
#include <cstring>
#include <fstream>
#include <iostream>
#include <iterator>

Canvas::Canvas(int w, int h, rgb bg) {
  this->WIDTH = w;
  this->HEIGHT = h;
  this->BG = bg;
  this->zbuffer = vector<float>();
  zbuffer.resize(WIDTH * HEIGHT, 0);
  canvas.resize(WIDTH * HEIGHT, bg);
  header = "P6\n" + to_string(WIDTH) + " " + to_string(HEIGHT) + " ";
  // SDL
  window_rect = {0, 0, WIDTH, HEIGHT};
}
// translates  2d coordinates from the normal coordinate system to how it looks
// in memory. (basically rotates the coordinate system 90°)
int Canvas::translatePoints(float x, float y) {
  int xn = (int)ceil(x);
  int yn = (int)ceil(y);
  yn = HEIGHT - yn - 1;

  return xn + WIDTH * yn;
}

void Canvas::clearZbuffer() {
  zbuffer.clear();
  zbuffer.resize(WIDTH * HEIGHT, 0);
}
void Canvas::setZbuffer(int x, int y, float z) {
  int point = translatePoints(x, y);
  if (point < 0 || point >= WIDTH * HEIGHT) {
    cout << "zbuffer return " << point;
    return;
  }
  zbuffer[point] = z;
};
void Canvas::setColour(rgb src, float x, float y) {
  int point = translatePoints(x, y);
  if (point < 0 || point >= WIDTH * HEIGHT) {
    return;
  }
  canvas[point].r = src.r;
  canvas[point].g = src.g;
  canvas[point].b = src.b;
}
void Canvas::setColour(rgb src, v3 p) {
  int point = translatePoints(p.x, p.y);
  if (point < 0 || point > WIDTH * HEIGHT) {
    return;
  }
  zbuffer[point] = p.z;
  canvas[point].r = src.r;
  canvas[point].g = src.g;
  canvas[point].b = src.b;
}
void Canvas::write(const char *filename) {
  ofstream file;
  file.open(filename, ios::binary | ios::out);
  file << header;
  file << "255\n";
  for (rgb byte : canvas) {
    file.write((char *)&byte, sizeof(byte));
  }
  file.close();
}

std::vector<float> interpolate(float i0, float d0, float i1, float d1) {
  if (i0 == i1) {
    return vector<float>{d0};
  }
  std::vector<float> values;
  float a = (d1 - d0) / (i1 - i0);
  float d = d0;

  for (float i = i0; i <= i1; ++i) {
    values.push_back(d);
    d += a;
  }
  if (values.size() == 0)
    return vector<float>{d0};

  return values;
}

vector<vector<float>> edgeInterpolate(float y0, float v0, float y1, float v1,
                                      float y2, float v2) {
  vector<float> v01 = interpolate(y0, v0, y1, v1);
  vector<float> v12 = interpolate(y1, v1, y2, v2);
  vector<float> v02 = interpolate(y0, v0, y2, v2);
  v01.pop_back();
  vector<float> v012 = v01;
  v012.insert(v012.end(), v12.begin(), v12.end());
  return vector<vector<float>>{v02, v012};
}

void Canvas::draw_triangle3d(Triangle triangle, rgb colour) {

  // float fov = 45.0f * (M_PI / 180.0f); // Convert FOV to radians
  // float aspectRatio = 1;    // Aspect ratio (width / height)
  // float zNear = 1.f;                  // Near clipping plane
  // float zFar = 10.0f;                 // Far clipping plane
  // v3 p1 = Utils::projectMatrix(v3(triangle.p1.x, triangle.p1.y,
  // triangle.p1.z), fov, aspectRatio, zNear, zFar); v3 p2 =
  // Utils::projectMatrix(v3(triangle.p2.x, triangle.p2.y, triangle.p2.z), fov,
  // aspectRatio, zNear, zFar); v3 p3 = Utils::projectMatrix(v3(triangle.p3.x,
  // triangle.p3.y, triangle.p3.z), fov, aspectRatio, zNear, zFar);
  v3 p1 = Utils::project(v3(triangle.p1.x, triangle.p1.y, triangle.p1.z));
  v3 p2 = Utils::project(v3(triangle.p2.x, triangle.p2.y, triangle.p2.z));
  v3 p3 = Utils::project(v3(triangle.p3.x, triangle.p3.y, triangle.p3.z));
  if (p2.y < p1.y)
    swap(p2, p1);
  if (p3.y < p1.y)
    swap(p3, p1);
  if (p3.y < p2.y)
    swap(p3, p2);
  auto xInterpolations = edgeInterpolate(p1.y, p1.x, p2.y, p2.x, p3.y, p3.x);
  // maybe not divide by 1?
  // p0.y, v0.z, p1.y, v1.z, p2.y, 1.0/v2.z);
  auto zInterpolations =
      edgeInterpolate(p1.y, 1.0 / p1.z, p2.y, 1.0 / p2.z, p3.y, 1.0 / p3.z);
  auto hInterpolations = edgeInterpolate(p1.y, p1.z, p2.y, p2.z, p3.y, p3.z);
  auto x02 = xInterpolations[0];
  auto x012 = xInterpolations[1];
  auto z02 = zInterpolations[0];
  auto z012 = zInterpolations[1];
  vector<float> x_left;
  vector<float> x_right;
  vector<float> iz_left;
  vector<float> iz_right;
  int m = floor(x02.size() / 2);
  if (x02[m] < x012[m]) {
    swap(x_left, x02);
    swap(x_right, x012);
    swap(iz_left, z02);
    swap(iz_right, z012);
  } else {
    swap(x_left, x012);
    swap(x_right, x02);
    swap(iz_left, z012);
    swap(iz_right, z02);
  }
  for (int y = p1.y; y <= p3.y; y++) {
    int index = round(y - p1.y);
    // cout << index << endl;
    if (index < 0)
      continue;
    float xl = x_left[index];
    float xr = x_right[index];
    // floaterpolate attributes for this scanline.
    float zl = iz_left[index];
    float zr = iz_right[index];
    xl = round(xl);
    xr = round(xr);
    vector<float> zscan =
        interpolate((float)xl, (float)zl, (float)xr, (float)zr);
    for (int x = xl; x <= xr; x++) {
      float zcheck = zscan[x - xl];
      // cout << zbuffer[translatePoints(x, y)] << " >= " << zcheck << endl;
      int tp = translatePoints(x, y);
      if (tp < 0 || tp >= WIDTH * HEIGHT)
        continue;
      // inverted cus 1/z
      if (zbuffer[tp] < zcheck) {
        setZbuffer(x, y, zcheck);
        setColour(colour, x, y);
      }
    }
  }
}

vector<v3> Canvas::draw_line3d_matrix(v3 p1, v3 p2, rgb colour, float fov,
                                      float aspectRatio, float zNear,
                                      float zFar) {
  p1 = Utils::projectMatrix(p1, fov, aspectRatio, zNear, zFar);
  p2 = Utils::projectMatrix(p2, fov, aspectRatio, zNear, zFar);
  return draw_line(p1.x, p1.y, p2.x, p2.y, 1, colour, v2{p1.z, p2.z});
}

vector<v3> Canvas::draw_line3d(v3 p1, v3 p2, rgb colour) {
  p1 = Utils::project(p1);
  p2 = Utils::project(p2);
  return draw_line(p1.x, p1.y, p2.x, p2.y, 1, colour, v2{p1.z, p2.z});
}

void Canvas::draw_line(v2 p1, v2 p2, int thickness, rgb colour) {
  draw_line(p1.x, p1.y, p2.x, p2.y, thickness, colour);
}

vector<v3> Canvas::draw_line(float x1, float y1, float x2, float y2,
                             int thickness, rgb colour, v2 zInfo) {
  // implement bresenham's optimized line drawing algorithmu. check
  // https://www.youtube.com/watch?v=IDFB5CDpLDE
  float dx = abs(x1 - x2);
  float dy = abs(y1 - y2);
  float dz = (zInfo.x - zInfo.y);
  float zInc;
  if (dx < dy) {
    zInc = dz / dy;
  } else {
    zInc = dz / dx;
  }
  float currZ = zInfo.y;
  Line orgLine = Line(v3{x1, y1, 1}, v3{x2, y2, 1}, colour);
  float rise = y2 - y1;
  float run = x2 - x1;
  vector<v3> vals = vector<v3>();
  if (run == 0) {
    if (y1 > y2) {
      float temp = y1;
      y1 = y2;
      y2 = temp;
    }
    for (float i = y1; i <= y2; i++) {
      vals.push_back(v3{x1, i, currZ});
      setColour(colour, v3(x1, i, currZ));
      currZ += zInc;
    }

  } else {
    double m = (float)rise / run;
    float adjust;
    if (m >= 0)
      adjust = 1;
    else
      adjust = -1;
    float offset = 0;

    if (m <= 1 && m >= -1) {
      float delta = abs(rise) * 2;
      float threshold = abs(run);
      float thresholdInc = abs(run) * 2;
      float y = y1;
      if (x2 < x1) {
        float temp = x1;
        x1 = x2;
        x2 = temp;
        y = y2;
      }
      for (float x = x1; x <= x2; x++) {
        vals.push_back(v3(x, y, currZ));
        currZ += zInc;
        setColour(colour, v3(x, y, currZ));
        offset += delta;
        if (offset >= threshold) {
          y += adjust;
          threshold += thresholdInc;
        }
        currZ += zInc;
      }
    } else {
      float delta = abs(run) * 2;
      float threshold = abs(rise);
      float thresholdInc = abs(rise) * 2;
      float x = x1;
      if (y2 < y1) {
        float temp = y1;
        y1 = y2;
        y2 = temp;
        x = x2;
      }
      for (float y = y1; y <= y2; y++) {
        vals.push_back(v3(x, y, currZ));
        currZ += zInc;
        setColour(colour, v3(x, y, currZ));
        offset += delta;
        if (offset >= threshold) {
          x += adjust;
          threshold += thresholdInc;
        }
        currZ += zInc;
      }
    }
    if (zInfo.x != 0 && zInfo.y != 0)
      return vals;
    return vals;
    // if (thickness > 1 && false) {
    //
    //   int recursions = floor(thickness / 2);
    //   //  ^
    //   // to create a line of certain thickness we add x amount to left and
    //   // right (or x )
    //   if (thickness % 2 == 0) {
    //     for (int i = 1; i <= recursions; i++) {
    //       Line newLine = orgLine;
    //       newLine.p1.x += i;
    //       newLine.p2.x += i;
    //       // draw_line(newLine);
    //     }
    //     for (int i = -1; i >= -(recursions - 1); i--) {
    //       Line newLine = orgLine;
    //       newLine.p1.x += i;
    //       newLine.p2.x += i;
    //       // draw_line(newLine);
    //     }
    //   } else {
    //     for (int i = 1; i <= thickness; i++) {
    //       Line newLine = orgLine;
    //       newLine.p1.x += i;
    //       newLine.p2.x += i;
    //       // draw_line(newLine);
    //     }
    //     for (int i = -1; i >= -(thickness); i--) {
    //       Line newLine = orgLine;
    //       newLine.p1.x += i;
    //       newLine.p2.x += i;
    //       // draw_line(newLine);
    //     }
    //   }
    // }
  }
  return vals;
}

// https://gabrielgambetta.com/computer-graphics-from-scratch/07-filled-triangles.html
void Canvas::fill_triangle(const Triangle &trig, rgb colour) {
  v3 p1 = trig.p1;
  v3 p2 = trig.p2;
  v3 p3 = trig.p3;
  if (p2.y < p1.y)
    swap(p2, p1);
  if (p3.y < p1.y)
    swap(p3, p1);
  if (p3.y < p2.y)
    swap(p3, p2);
}

void Canvas::fill_between_lines(Line line1, Line line2, rgb colour) {
  float bottomY;
  float topY;
  if (line1.bottomY >= line2.bottomY)
    bottomY = line1.bottomY;
  else
    bottomY = line2.bottomY;
  if (line1.topY <= line2.topY)
    topY = line1.topY;
  else
    topY = line2.topY;
  for (float y = bottomY; y < topY; y++) {
    float x1 = line1.xFromY(y);
    float x2 = line2.xFromY(y);
    draw_line(x1, y, x2, y, 1, colour);
  }
}
Triangle Canvas::draw_triangle(float x1, float y1, float x2, float y2, float x3,
                               float y3, int edgeThickness, rgb colour,
                               int fill, rgb fillColour) {

  Triangle trig = Triangle(v3{x1, y1, 1}, v3{x2, y2, 1}, v3{x3, y3, 1});
  if (fill) {
    vector<Line> lines = trig.getLongestEdge();
    fill_between_lines(lines[0], lines[1], fillColour);
    fill_between_lines(lines[0], lines[2], fillColour);
  }
  // draw_line(trig.a, edgeThickness, colour);
  // draw_line(trig.b, edgeThickness, colour);
  // draw_line(trig.c, edgeThickness, colour);
  return trig;
}
void display_vector(const vector<v3> &v) {
  std::copy(v.begin(), v.end(), std::ostream_iterator<v3>(std::cout, "\n"));
}

void Canvas::draw_triangle(Triangle trig, int edgeThickness, rgb colour,
                           int fill, rgb fillColour) {
  vector<Line> lines = trig.getLongestEdge();
  // if (fill) {
  //   fill_between_lines(lines[0], lines[1], fillColour);
  //   fill_between_lines(lines[0], lines[2], fillColour);
  // }
  // draw_triangle(triangle.a,triangle.b,triangle.c,edgeThickness,colour,fill,fillColour);
  auto vals1 = draw_line(trig.a.p1.x, trig.a.p1.y, trig.a.p2.x, trig.a.p2.y,
                         edgeThickness, colour);
  auto vals2 = draw_line(trig.b.p1.x, trig.b.p1.y, trig.b.p2.x, trig.b.p2.y,
                         edgeThickness, colour);
  auto vals3 = draw_line(trig.c.p1.x, trig.c.p1.y, trig.c.p2.x, trig.c.p2.y,
                         edgeThickness, colour);
}

Triangle Canvas::draw_triangle(Line a, Line b, Line c, int edgeThickness,
                               rgb colour, int fill, rgb fillColour) {

  Triangle trig = Triangle(a, b, c);
  if (fill) {
    vector<Line> lines = trig.getLongestEdge();
    fill_between_lines(lines[0], lines[1], fillColour);
    fill_between_lines(lines[0], lines[2], fillColour);
  }
  // draw_line(a, edgeThickness, colour);
  // draw_line(b, edgeThickness, colour);
  // draw_line(c, edgeThickness, colour);
  return trig;
}

void Canvas::translate(Triangle &triangle, float x, float y, int fill,
                       rgb colour) {
  draw_triangle(triangle, 1, BG);
  triangle.translate(x, y, 0);
  draw_triangle(triangle, fill, colour);
}

int Canvas::getSize() { return canvas.size(); }
void Canvas::rgb_to_rgba(uint32_t *pixels) {
  uint32_t *rgba = new uint32_t[canvas.size()];
  uint32_t *temp = new uint32_t;

  for (size_t i = 0; i < canvas.size(); i++) {
    // reverse order!
    uint8_t arr[4] = {canvas[i].r, canvas[i].g, canvas[i].b, 255};
    memcpy(temp, arr, sizeof(uint8_t) * 4);
    rgba[i] = *temp;
  }

  memcpy(pixels, rgba, canvas.size() * sizeof(uint32_t));
  delete[] rgba;
  delete temp;
}
void Canvas::clear() {
  canvas.clear();
  canvas.resize(WIDTH * HEIGHT, this->BG);
}

void Canvas::SDL_Init(const char *win_name) {
  SDL_Window *window = NULL;
  window = SDL_CreateWindow("test", 0, 0, 0, 0, 0);
  if (window == NULL)
    throwErr("Failed to create window");
  renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
  texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA32,
                              SDL_TEXTUREACCESS_STREAMING, WIDTH, HEIGHT);
  if (texture == NULL)
    throwErr("Texture creation failed");
  SDL_SetWindowSize(window, WIDTH, HEIGHT);
}

void Canvas::SDL_Init(SDL_Renderer *renderer) {
  this->renderer = renderer;
  texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA32,
                              SDL_TEXTUREACCESS_STREAMING, WIDTH, HEIGHT);
  if (texture == NULL)
    throwErr("Texture creation failed");
}

void Canvas::Render_SDL() {
  void *pixels_dst;
  int pitch;
  if (SDL_LockTexture(texture, &window_rect, &pixels_dst, &pitch) < 0)
    throwErr("Failed to lock texture");
  uint32_t *pixels = new uint32_t[getSize()];
  rgb_to_rgba(pixels);
  memcpy(pixels_dst, pixels, getSize() * sizeof(uint32_t));
  SDL_UnlockTexture(texture);
  if (SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0) < 0)
    throwErr("Failed to draw color");
  if (SDL_RenderClear(renderer) < 0)
    throwErr("Failed to clear renderer");
  if (SDL_RenderCopy(renderer, texture, &window_rect, &window_rect) < 0)
    throwErr("Failed to copy tecture");
  SDL_RenderPresent(renderer);
  delete[] pixels;
}
void Canvas::drawAllCircleSides(float x, float y, Circle circle, int fill,
                                rgb fillColour) {
  float xc = circle.xc;
  float yc = circle.yc;
  rgb colour = circle.colour;
  setColour(colour, xc + x, yc + y);
  setColour(colour, xc - x, yc + y);
  setColour(colour, xc + x, yc - y);
  setColour(colour, xc - x, yc - y);
  setColour(colour, xc + y, yc + x);
  setColour(colour, xc - y, yc + x);
  setColour(colour, xc + y, yc - x);
  setColour(colour, xc - y, yc - x);
  if (fill) {
    draw_line(xc + x, yc + y, xc - x, yc + y, 1, fillColour);
    draw_line(xc + x, yc - y, xc - x, yc - y, 1, fillColour);
    draw_line(xc + y, yc + x, xc - y, yc + x, 1, fillColour);
    draw_line(xc + y, yc - x, xc - y, yc - x, 1, fillColour);
  }
}

void Canvas::draw_circle(v2 v, float r, int fill, rgb colour, rgb fillColour) {
  draw_circle(v.x, v.y, r, fill, colour, fillColour);
}
void Canvas::draw_circle(float xc, float yc, float r, int fill, rgb colour,
                         rgb fillColour) {
  Circle circle = Circle(xc, yc, r, colour);
  float x = 0, y = r;
  float d = 3 - 2 * r;
  drawAllCircleSides(x, y, circle, fill, fillColour);
  while (y >= x) {
    x++;
    if (d > 0) {
      y--;
      d = d + 4 * (x - y) + 10;
    } else
      d = d + 4 * x + 6;

    drawAllCircleSides(x, y, circle, fill, fillColour);
  }
}
void Canvas::renderText(TTF_Font *font, const string &text, int x, int y,
                        SDL_Color color) {
  Utils::renderText(renderer, font, text, x, HEIGHT - y, color);
}
