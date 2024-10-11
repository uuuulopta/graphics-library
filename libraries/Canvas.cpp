#include "../headers/Canvas.h"
#include <cstring>
#include <fstream>

Canvas::Canvas(int w, int h, rgb bg) {
  this->WIDTH = w;
  this->HEIGHT = h;
  this->BG = bg;
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
void Canvas::setColour(rgb src, float x, float y) {
  int point = translatePoints(x, y);
  if (point < 0 || point > WIDTH * HEIGHT) {
    return;
  }
  canvas[point].r = src.r;
  canvas[point].g = src.g;
  canvas[point].b = src.b;
}
void Canvas::setColour(rgb src, v3 p) {
  int point = translatePoints(p.x, p.y);
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

void Canvas::draw_line3d(v3 p1, v3 p2, rgb colour) {
  Line l = Line(p1, p2, colour);
  draw_line(l, 1, colour);
}
Line Canvas::draw_line(float x1, float y1, float x2, float y2, int thickness,
                       rgb colour) {
  // implement bresenham's optimized line drawing algorithmu. check
  // https://www.youtube.com/watch?v=IDFB5CDpLDE
  Line orgLine = Line(v3{x1, y1, 1}, v3{x2, y2, 1}, colour);
  float rise = y2 - y1;
  float run = x2 - x1;
  if (run == 0) {
    if (y1 > y2) {
      float temp = y1;
      y1 = y2;
      y2 = temp;
    }
    for (float i = y1; i <= y2; i++) {
      setColour(colour, x1, i);
    }
  } else {
    double m = (double)rise / run;
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
        setColour(colour, x, y);
        offset += delta;
        if (offset >= threshold) {
          y += adjust;
          threshold += thresholdInc;
        }
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
        setColour(colour, x, y);
        offset += delta;
        if (offset >= threshold) {
          x += adjust;
          threshold += thresholdInc;
        }
      }
    }
  }
  if (thickness > 1) {

    int recursions = floor(thickness / 2);
    //  ^
    // to create a line of certain thickness we add x amount to left and right
    // (or x )
    if (thickness % 2 == 0) {
      for (int i = 1; i <= recursions; i++) {
        Line newLine = orgLine;
        newLine.p1.x += i;
        newLine.p2.x += i;
        draw_line(newLine);
      }
      for (int i = -1; i >= -(recursions - 1); i--) {
        Line newLine = orgLine;
        newLine.p1.x += i;
        newLine.p2.x += i;
        draw_line(newLine);
      }
    } else {
      for (int i = 1; i <= thickness; i++) {
        Line newLine = orgLine;
        newLine.p1.x += i;
        newLine.p2.x += i;
        draw_line(newLine);
      }
      for (int i = -1; i >= -(thickness); i--) {
        Line newLine = orgLine;
        newLine.p1.x += i;
        newLine.p2.x += i;
        draw_line(newLine);
      }
    }
  }

  return orgLine;
}
void Canvas::draw_line(Line line, int thickness, rgb colour) {
  draw_line(line.p1.x, line.p1.y, line.p2.x, line.p2.y, thickness, colour);
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
  draw_line(trig.a, edgeThickness, colour);
  draw_line(trig.b, edgeThickness, colour);
  draw_line(trig.c, edgeThickness, colour);
  return trig;
}
void Canvas::draw_triangle(Triangle trig, int edgeThickness, rgb colour,
                           int fill, rgb fillColour) {
  vector<Line> lines = trig.getLongestEdge();
  if (fill) {
    fill_between_lines(lines[0], lines[1], fillColour);
    fill_between_lines(lines[0], lines[2], fillColour);
  }
  // draw_triangle(triangle.a,triangle.b,triangle.c,edgeThickness,colour,fill,fillColour);
  draw_line(trig.a, edgeThickness, colour);
  draw_line(trig.b, edgeThickness, colour);
  draw_line(trig.c, edgeThickness, colour);
}
Triangle Canvas::draw_triangle(Line a, Line b, Line c, int edgeThickness,
                               rgb colour, int fill, rgb fillColour) {

  Triangle trig = Triangle(a, b, c);
  if (fill) {
    vector<Line> lines = trig.getLongestEdge();
    fill_between_lines(lines[0], lines[1], fillColour);
    fill_between_lines(lines[0], lines[2], fillColour);
  }
  draw_line(a, edgeThickness, colour);
  draw_line(b, edgeThickness, colour);
  draw_line(c, edgeThickness, colour);
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
