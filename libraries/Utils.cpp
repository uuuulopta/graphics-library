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
  return v3((p.x) / p.z, (p.y) / p.z, p.z);
}

/*
-1 ..x... 1  => 0 .x+1.. 1 =>

*/
v3 Utils::normalized_to_screen(v3 p) {
  float x = round(((p.x + 1) / 2) * WIDTH);
  float y = round(((p.y + 1) / 2) * HEIGHT);
  return v3(x, y, p.z);
}

v2 Utils::normalized_to_screen(v2 p) {
  float x = round(((p.x + 1) / 2) * WIDTH);
  float y = round(((p.y + 1) / 2) * HEIGHT);
  return v2{x, y};
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
v3 Utils::project(v3 p,v3 camera){
  float nx=p.x - camera.x;
  float ny=p.y - camera.y;
  float nz=p.z - camera.z;
  return normalized_to_screen(v3(nx/nz,ny/nz,nz));
}
v2 Utils::projectMatrix(const v3& point, float fov, float aspectRatio, float zNear, float zFar) {
    v2 projected;

    float tanHalfFOV = std::tan(fov / 2.0f);
    projected.x = point.x / (aspectRatio * tanHalfFOV * ( point.z + 3 ));
    projected.y = ( point.y ) / (tanHalfFOV * ( point.z +3 ));
    // float zProjected = (zFar + zNear) / (zNear - zFar) * point.z + (2.0f * zFar * zNear) / (zNear - zFar);

    return normalized_to_screen(projected);
}

Line Utils::project_line(Line l) {
  v3 p1 = Utils::project(v3(l.p1.x, l.p1.y, l.p1.z));
  v3 p2 = Utils::project(v3(l.p2.x, l.p2.y, l.p2.z));
  return Line(p1, p2, l.colour);
}
v2 Utils::normalize_scale(v2 v,float scale){ return normalized_to_screen(v / scale); }
v3 Utils::normalize_scale(v3 v,float scale){
  return normalized_to_screen(Utils::project_2d_3d({v.x/scale,v.y/scale,v.z})); 
}
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
