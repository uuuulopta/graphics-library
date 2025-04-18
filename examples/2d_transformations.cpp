#include <SDL2/SDL.h>
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_keyboard.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_scancode.h>
#include <SDL2/SDL_stdinc.h>
#include <fstream>
#include <functional>
#include <iostream>
#include <string>
#include <vector>
// #include "windows.h"
#include "../headers/Canvas.h"
#include "../headers/Example.h"
#include "../headers/Utils.h"
#include "square.h"
using namespace std;

// vscode formatted it like this :(

const int SLIDES_COUNT = 5;
enum class Slide {Translation=1,Rotation,Scaling,ShearingX,ShearingY};

void drawCordSystem(vector<v2> xAxis, vector<v2> yAxis,Canvas &c,float scale){
    v2 leftX  = Utils::normalize_scale(xAxis[0] , scale);
    v2 rightX = Utils::normalize_scale(xAxis.back() , scale);
    v2 downY  = Utils::normalize_scale(yAxis[0] , scale);
    v2 upY    = Utils::normalize_scale(yAxis.back() , scale);
    c.draw_line(leftX,rightX,1,BLACK);
    c.draw_line(downY,upY,1,BLACK);
    for(int i = 0;i < xAxis.size();i++){
      c.draw_circle(Utils::normalize_scale(xAxis[i],scale), 2,1, RED,RED);
      c.draw_circle(Utils::normalize_scale(yAxis[i],scale), 2,1, RED,RED);
    }
}

Slide nextSlide(Slide current){
  int next = (int) current + 1;
  if ( next > SLIDES_COUNT ) return (Slide) 1;
  return (Slide) next;
}
Slide lastSlide(Slide current){
  int last = (int) current - 1;
  if ( last < 1 ) return (Slide) 1;
  return (Slide) last;
}


void transformations_2d::main() {
  Canvas c = Canvas(WIDTH, HEIGHT);
  c.SDL_Init(renderer);
  c.Render_SDL();
  SDL_Event event;
  vector<v2> xAxis = vector<v2>();
  vector<v2> yAxis = vector<v2>();
  const float scale = 10;
  for(int i = -scale; i <= scale; i++){
    xAxis.push_back(v2(i,0));
    yAxis.push_back(v2(0,i));
  }
  Slide currentSlide = Slide::Translation;
  bool initializeSlide = true;
  Square2d square = Square2d(2, c,10);
  square.setMovment(Direction::left);
  float dt;
  uint32_t last = SDL_GetTicks();
  uint32_t start = SDL_GetTicks();
  bool quit = false;
  cout << endl << endl;
  auto fontSmall = TTF_OpenFont("assets/DejaVuSans.ttf", 11);
  while (!quit) { 
    SDL_PollEvent(&event);
    if(event.type == SDL_QUIT) exit(0);
    start = SDL_GetTicks();
    dt = (start - last) / 1000.f;
    last = start;
    if (dt <= 0.0) continue; 
    drawCordSystem(xAxis, yAxis,c,scale);
    if(event.type == SDL_KEYDOWN)
    {
      switch(event.key.keysym.scancode){
        case SDL_SCANCODE_RIGHT:
          currentSlide = nextSlide(currentSlide);
          initializeSlide = true;
          break;
        case SDL_SCANCODE_LEFT:
          currentSlide = lastSlide(currentSlide);
          initializeSlide = true;
          break;
        default:
          break;
      }
    }
    if(initializeSlide){

      square.setMovment(Direction::idle);
      switch (currentSlide){
        case Slide::Translation:
          square = Square2d(2, c,scale);
          square.setMovment(Direction::left);
          square.translateSquares(2, 2);
          break;
        case Slide::Rotation:
          square = Square2d(2, c,scale);
          break;
        case Slide::Scaling:
          square = Square2d(2, c,scale);
          square.translateSquares(-1, -1);
          square.setScalingDirection(ScaledDirection::expand);
          break;
        case Slide::ShearingY:
        case Slide::ShearingX:
          square = Square2d(2, c,scale);
          break;  
      }
      initializeSlide = false;
    } 
    switch(currentSlide){
      case Slide::Translation:
        square.animateTranslation(dt);
        break;
      case Slide::Rotation:
        square.animateRotation(dt);
        break;
      case Slide::Scaling:
        square.animateScaling(dt);
        break;
      case Slide::ShearingX:
        square.animateShearing(dt,ShearAnimation::x);
        break;  
      case Slide::ShearingY:
        square.animateShearing(dt,ShearAnimation::y);
        break;  
    }
    runBeforeRender(quit);
    c.Render_SDL();
    square.writeCords(fontSmall);
    SDL_RenderPresent(renderer);
    c.clear();
  }
}
