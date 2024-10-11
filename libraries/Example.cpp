#include "../headers/Example.h"
#include "../headers/Utils.h"
#include <SDL2/SDL_keyboard.h>
#include <SDL2/SDL_scancode.h>

Example::Example(int width, int height, SDL_Renderer *renderer,
                 TTF_Font *font) {
  this->WIDTH = width;
  this->HEIGHT = height;
  this->renderer = renderer;
  this->font = font;
}
void Example::runBeforeRender(bool &quit) {
  Utils::renderText(renderer, font, "ESC za izlaz", 0, 0, {0, 0, 255});
  if (SDL_GetKeyboardState(nullptr)[SDL_SCANCODE_ESCAPE]) {
    quit = true;
  }
}
