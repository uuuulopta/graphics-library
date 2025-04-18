#include <SDL2/SDL.h>
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_keyboard.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_scancode.h>
#include <SDL2/SDL_stdinc.h>
#include <algorithm>
#include <fstream>
#include <functional>
#include <iostream>
#include <string>
#include <vector>
// #include "windows.h"
#include "../headers/Canvas.h"
#include "../headers/Example.h"
#include "../headers/Utils.h"
#include "../headers/objparser.h"
#include "square.h"
using namespace std;


void transformations_3d::drawCordSystem(vector<v3> xAxis, vector<v3> yAxis,vector<v3> zAxis,Canvas &c,float scale){
    v3 leftX  = Utils::normalize_scale(xAxis[0],scale) ;
    v3 rightX = Utils::normalize_scale(xAxis.back(),scale) ;
    v3 downY  = Utils::normalize_scale(yAxis[0],scale) ;
    v3 upY    = Utils::normalize_scale(yAxis.back(),scale) ;

    v3 inZ    = Utils::project_2d_3d(v3(zAxis[0].x / scale, zAxis[0].y / scale,zAxis[0].z / scale ));
    inZ    = Utils::normalized_to_screen(inZ);

    v3 outZ    = Utils::project_2d_3d(v3(zAxis.back().x / scale, zAxis.back().y / scale,zAxis.back().z / scale ));
    outZ    = Utils::normalized_to_screen(outZ);

    c.draw_line(leftX.x, leftX.y, rightX.x, rightX.y, 1, RED, v2{leftX.z, rightX.z});
    c.draw_line(downY.x, downY.y, upY.x, upY.y, 1, BLUE, v2{downY.z, upY.z});
    // c.draw_line(inZ.x, inZ.y, outZ.x, outZ.y, 1, GREEN, v2{inZ.z, outZ.z});
    // c.draw_circle(inZ.x,inZ.y,2,1,RED,RED);
    for(int i = 0;i < xAxis.size();i++){
      // v3 projectedY = Utils::project_2d_3d(yAxis[i]); 
      // v3 px = Utils::normalize_scale(Utils::project_2d_3d(xAxis[i]),scale);
      // v3 py = Utils::normalize_scale(projectedY,scale);
      // c.draw_circle(px.x,px.y, 2,1, RED,RED);
      // c.draw_circle(py.x,py.y, 2,1, GREEN,GREEN);
    }
}

Slide3D transformations_3d::nextSlide(Slide3D current){
  int next = (int) current + 1;
  if ( next > SLIDES_COUNT3D ) return (Slide3D) 1;
  return (Slide3D) next;
}
Slide3D transformations_3d::lastSlide(Slide3D current){
  int last = (int) current - 1;
  if ( last < 1 ) return (Slide3D) 1;
  return (Slide3D) last;
}


void transformations_3d::main() {
  Canvas c = Canvas(WIDTH, HEIGHT);
  c.SDL_Init(renderer);
  c.Render_SDL();
  SDL_Event event;
  vector<v3> xAxis = vector<v3>();
  vector<v3> yAxis = vector<v3>();
  vector<v3> zAxis = vector<v3>();
  const float scale = 1;
  for(int i = -scale; i <= scale; i++){
    xAxis.push_back(v3(i,1,3));
    yAxis.push_back(v3(1,i,3));
  }
  for(int i = scale; i <= scale*30; i++){
    zAxis.push_back(v3(1,1,i));
  }
  for(auto &x : xAxis) {
    x.rotate(Axis3d::y, {0,0,0}, 0.785398);
     x.translate(-0.25, 0, 0);
  }
  for(auto &y : yAxis) {
    // y.rotate(Axis3d::y, {0,0,0}, 0.785398);
     y.translate(-0.25, 0, 0);
  }
  for(auto &z : zAxis) {
    z.rotate(Axis3d::y, {0,0,0}, 0.785398);
    z.translate(-1, -1, 0);
    // z.rotate(Axis3d::y, {0,0,0}, 0.985398);

  }

  Slide3D currentSlide = Slide3D::Translation;
  bool initializeSlide = true;
  // Square2d square = Square2d(2, c,10);
  // square.setMovment(Direction::left);
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
    v3 pxLeft =  v3(-10,0,1);
    v3 pxRight = v3(10,0,1);
    v3 pyDown =  v3(0,-10,1);
    v3 pyUp =    v3(0,10,1);
    v3 pzClose =    v3(0,0,1);
    v3 pzFar =    v3(0,0,10);


      float zmin = max({pxLeft.z,pxRight.z,pzClose.z,pzFar.z,pyUp.z,pyDown.z});
    float xmax = max({pxLeft.x,pxRight.x,pzClose.x,pzFar.x}) ;
    float xmin =  min({pxLeft.x,pxRight.x,pzClose.x,pzFar.x}) ;
    float ymax = max({pxLeft.y,pxRight.y,pzClose.y,pzFar.y,pyUp.y,pyDown.y});
    float ymin = min({pxLeft.y,pxRight.y,pzClose.y,pzFar.y,pyUp.y,pyDown.y});;
    float zmax = max({pxLeft.z,pxRight.z,pzClose.z,pzFar.z,pyUp.z,pyDown.z});
    pxLeft   = mapPoint3d( pxLeft,  xmax,  xmin,  ymax,  ymin,  zmax,  zmin);
    pxRight =mapPoint3d( pxRight,  xmax,  xmin,  ymax,  ymin,  zmax,  zmin);
    pyDown  = mapPoint3d( pyDown,  xmax,  xmin,  ymax,  ymin,  zmax,  zmin);
    pzClose    =   mapPoint3d( pzClose,  xmax,  xmin,  ymax,  ymin,  zmax,  zmin);
    pzFar    =   mapPoint3d( pzFar,  xmax,  xmin,  ymax,  ymin,  zmax,  zmin);
    float avgx  = ( pxLeft.x + pxRight.x + pzClose.x + pzFar.x + pyUp.x + pyDown.x ) / 6;
    float avgy  = ( pxLeft.y + pxRight.y + pzClose.y + pzFar.y + pyUp.y + pyDown.y ) / 6;
    float avgz  = ( pxLeft.z + pxRight.z + pzClose.z + pzFar.z + pyUp.z + pyDown.z ) / 6;
    pxRight.rotate(Axis3d::y, {avgx,avgy,avgz}, 0.785398);
    pxLeft.rotate(Axis3d::y, {avgx,avgy,avgz},  0.785398);
    pzClose.rotate(Axis3d::y, {avgx,avgy,avgz}, 0.785398);
    pzFar.rotate(Axis3d::y, {avgx,avgy,avgz},   0.785398);
  float zt = max(ymax, xmax);
  // translate so the object fits into the viewport
  pxLeft.translate( -0.0, 0, 1 + zt);
  pxLeft.translate( -0.0, 0, 1 + zt); 
  pxRight.translate( -0.0, 0, 1 + zt); 
  pyDown .translate( -0.0, 0, 1 + zt); 
  pyUp   .translate( -0.0, 0, 1 + zt);
 cout << pxLeft << endl << pxRight  << endl << pyDown << endl << pyUp << pzFar << endl << pzClose << endl; 
cout << "------------------------------------------------------------" << endl;
      // pxLeft.rotate(Axis3d::y, {0,0,10}, 0.785398);
    // pxRight.rotate(Axis3d::y, {0,0,10}, 0.785398);
    c.draw_line3d(pxLeft, pxRight, RED);
    c.draw_line3d(pyDown, pyUp, BLUE);
    c.draw_line3d(pzClose, pzFar, GREEN);
    // drawCordSystem(xAxis, yAxis,zAxis,c,scale);
    if(event.type == SDL_KEYDOWN)
    {
      switch(event.key.keysym.scancode){
        case SDL_SCANCODE_RIGHT:
          currentSlide = nextSlide(currentSlide);
          cout << "Switching to slide " << currentSlide;
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
    // if(initializeSlide){
    //
    //   square.setMovment(Direction::idle);
    //   switch (currentSlide){
    //     case Translation:
    //       square = Square2d(2, c,scale);
    //       square.setMovment(Direction::left);
    //       square.translateSquares(2, 2);
    //       break;
    //     case Rotation:
    //       square = Square2d(2, c,scale);
    //       break;
    //     case Scaling:
    //       square = Square2d(2, c,scale);
    //       square.translateSquares(-1, -1);
    //       square.setScalingDirection(ScaledDirection::expand);
    //       break;
    //     case ShearingY:
    //     case ShearingX:
    //       square = Square2d(2, c,scale);
    //       break;  
    //   }
    //   initializeSlide = false;
    // } 
    // switch(currentSlide){
    //   case Translation:
    //     square.animateTranslation(dt);
    //     break;
    //   case Rotation:
    //     square.animateRotation(dt);
    //     break;
    //   case Scaling:
    //     square.animateScaling(dt);
    //     break;
    //   case ShearingX:
    //     square.animateShearing(dt,ShearAnimation::x);
    //     break;  
    //   case ShearingY:
    //     square.animateShearing(dt,ShearAnimation::y);
    //     break;  
    // }
    runBeforeRender(quit);
    c.Render_SDL();
    // square.writeCords(fontSmall);
    SDL_RenderPresent(renderer);
    c.clear();
  }
}
