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
