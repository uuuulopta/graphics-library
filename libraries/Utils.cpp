#include "../headers/Utils.h"
#define WIDTH 800
#define HEIGHT 800

v3 Utils::getMostLeftPoint(vector<v3> points) {
  v3 least = points[0];
  for (v3 p : points) {
    if (p.x < least.x)
      least = p;
  }
  return least;
}

v3 Utils::project_2d_3d(v3 p) {
  if (p.z < 0)
    p.z = -p.z;
  return v3((p.x) / p.z, (p.y) / p.z, z);
}

/*
-1 ..x... 1  => 0 .x+1.. 1 =>

*/
v3 Utils::normalized_to_screen(v3 p) {
  float x = ((p.x + 1) / 2) * WIDTH;
  float y = ((p.y + 1) / 2) * HEIGHT;
  return v3(x, y, p.z);
}

vector<v3> Utils::getUniqueV3(vector<v3> vectors) {
  vector<v3> uniques = vector<v3>();
  for (v3 &v : vectors) {
    bool added = false;
    for (v3 &addedV3 : uniques) {
      if (addedV3 == v) {
        added = true;
        break;
      };
    }
    if (added)
      continue;
    uniques.push_back(v);
  }
  return uniques;
}

v3 Utils::project(v3 p) { return normalized_to_screen(project_2d_3d(p)); }

void Utils::renderText(SDL_Renderer *renderer, TTF_Font *font,
                       const string &text, int x, int y, SDL_Color color) {
  SDL_Surface *textSurface = TTF_RenderText_Solid(font, text.c_str(), color);
  SDL_Texture *textTexture =
      SDL_CreateTextureFromSurface(renderer, textSurface);
  SDL_Rect renderQuad = {x, y, textSurface->w, textSurface->h};

  SDL_RenderCopy(renderer, textTexture, nullptr, &renderQuad);

  SDL_FreeSurface(textSurface);
  SDL_DestroyTexture(textTexture);
}
