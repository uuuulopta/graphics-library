#include "headers/Example.h"
#include "headers/Utils.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <iostream>
#include <string>
using namespace std;

const int WIDTH = 800;
const int HEIGHT = 800;

// const int HEIGHT = 720;
// const int WIDTH = 1280;

bool init(SDL_Window **window, SDL_Renderer **renderer);
void close(SDL_Window *window, SDL_Renderer *renderer, TTF_Font *font);

int main(int argc, char *argv[]) {
  SDL_Window *window = nullptr;
  SDL_Renderer *renderer = nullptr;
  TTF_Font *font = nullptr;

  if (!init(&window, &renderer)) {
    cerr << "Failed to initialize window!" << endl;
    return -1;
  }

  if (TTF_Init() == -1) {
    cerr << "Failed to initialize SDL_ttf! TTF Error: " << TTF_GetError()
         << endl;
    return -1;
  }

  font = TTF_OpenFont("assets/DejaVuSans.ttf", 28);
  if (font == nullptr) {
    cerr << "Failed to load font! TTF Error: " << TTF_GetError() << endl;
    return -1;
  }

  bool quit = false;
  SDL_Event e;
  int selectedItem = 0;
  const int totalItems = 6;
  string menuItems[totalItems] = {"Transformations 2D", "Transformations 3D",
                                  "Cube.obj",           "Cup.obj",
                                  "Teapot.obj",         "Izlaz"};

  while (!quit) {
    while (SDL_PollEvent(&e) != 0) {
      if (e.type == SDL_QUIT) {
        quit = true;
      } else if (e.type == SDL_KEYDOWN) {
        switch (e.key.keysym.sym) {
        case SDLK_UP:
          selectedItem = (selectedItem - 1 + totalItems) % totalItems;
          break;
        case SDLK_DOWN:
          selectedItem = (selectedItem + 1) % totalItems;
          break;
        case SDLK_RETURN:
          cout << menuItems[selectedItem] << endl;
          switch (selectedItem) {
          case 0:
            // TranslateScale2d(WIDTH, HEIGHT, renderer, font).main();
            shear2d(WIDTH, HEIGHT, renderer, font).main();
            break;
          case 1:
            CubeProjection(WIDTH, HEIGHT, renderer, font).main();
            break;
          case 2:
            Obj("assets/cube.obj", WIDTH, HEIGHT, renderer, font).main();
            break;
          case 3:
            Obj("assets/solja.obj", WIDTH, HEIGHT, renderer, font).main();
            break;
          case 4:
            Obj("assets/teapot.obj", WIDTH, HEIGHT, renderer, font).main();
            break;
          case 5:
            close(window, renderer, font);
            return 0;
          }
          break;
        }
      }
    }

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    Utils::renderText(renderer, font, "Izaberite primer!", 100, 0,
                      {255, 255, 255});
    for (int i = 0; i < totalItems; ++i) {
      SDL_Color color = {255, 255, 255};
      if (i == selectedItem) {
        color = {255, 0, 0};
      }
      Utils::renderText(renderer, font, menuItems[i], 100, 100 + i * 50, color);
    }

    SDL_RenderPresent(renderer);
  }

  close(window, renderer, font);

  return 0;
}

bool init(SDL_Window **window, SDL_Renderer **renderer) {
  if (SDL_Init(SDL_INIT_VIDEO) < 0) {
    cerr << "SDL could not initialize! SDL Error: " << SDL_GetError() << endl;
    return false;
  }

  *window =
      SDL_CreateWindow("Menu", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
                       WIDTH, HEIGHT, SDL_WINDOW_SHOWN);
  if (*window == nullptr) {
    cerr << "Window could not be created! SDL Error: " << SDL_GetError()
         << endl;
    return false;
  }

  *renderer = SDL_CreateRenderer(*window, -1, SDL_RENDERER_ACCELERATED);
  if (*renderer == nullptr) {
    cerr << "Renderer could not be created! SDL Error: " << SDL_GetError()
         << endl;
    return false;
  }

  return true;
}

void close(SDL_Window *window, SDL_Renderer *renderer, TTF_Font *font) {
  TTF_CloseFont(font);
  font = nullptr;

  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  renderer = nullptr;
  window = nullptr;

  TTF_Quit();
  SDL_Quit();
}
