#pragma once
#include "Canvas.h"
#include "Shapes.h"
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_ttf.h>
#include <string>
class Example {
public:
  int WIDTH;
  int HEIGHT;
  SDL_Renderer *renderer;
  TTF_Font *font;
  virtual void main() = 0;
  void runBeforeRender(bool &quit);
  Example(int width, int height, SDL_Renderer *renderer, TTF_Font *font);
  virtual ~Example() {}
};

class Teapot : public Example {
public:
  using Example::Example;
  void main() override;
};
class Obj : public Example {
public:
  using Example::Example;
  Obj(std::string path_to_obj, int WIDTH, int HEIGHT, SDL_Renderer *renderer,
      TTF_Font *font);
  void main() override;

private:
  std::string path;
};
class Trianglex : Example {
public:
  using Example::Example;
  void main() override;
};

class CubeOrtho : Example {
public:
  using Example::Example;
  void main() override;
};

class CubeDots : Example {
public:
  using Example::Example;
  void main() override;
};

class CubeProjection : Example {
public:
  using Example::Example;
  void main() override;
};

class transformations_2d : Example {
public:
  using Example::Example;
  void main() override;
};

class TranslateScale2d : Example {
public:
  using Example::Example;
  void main() override;
};

class rotate2d : Example {
public:
  using Example::Example;
  void main() override;
};

class shear2d : Example {
public:
  using Example::Example;
  void main() override;
};

enum Slide3D { Translation = 1, Rotation, Scaling, ShearingX, ShearingY };
class transformations_3d : Example {
public:
  using Example::Example;
  void main() override;
  void drawCordSystem(vector<v3> xAxis, vector<v3> yAxis, vector<v3> zAxis,
                      Canvas &c, float scale);
  const int SLIDES_COUNT3D = 5;
  Slide3D nextSlide(Slide3D current);
  Slide3D lastSlide(Slide3D current);
};
