#include <SDL2/SDL_keyboard.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_scancode.h>
#include <SDL2/SDL_stdinc.h>
#include <cassert>
#include <csignal>
#include <functional>
#include <iostream>
#include <string>
// #include "windows.h"
#include "../headers/Canvas.h"
#include "../headers/Utils.h"
#include "../headers/colours.h"
using namespace std;

struct movment{
	int directionX,directionY;
};
enum class Direction{ left,right,up,down,diagonalUp,diagonalDown,idle};
enum class ScaledDirection{expand,shrink,none};
enum class ShearDirection{positive,negative};
enum class ShearAnimation{y,x,both};

class Square2d{
private:
	movment mov = {0,0}; 
	ScaledDirection scaleDir = ScaledDirection::none;
	ShearDirection shearDir = ShearDirection::positive;
	float scale = 1;
	Canvas *c;
	v2 squares[4];
	float length;


public:
	Square2d(float squareLength,Canvas &c,float scale){
		this->c = &c;
		length = squareLength;
		squares[0] = v2(0,0);
		squares[1] = v2(squareLength,0);
		squares[2] = v2(squareLength,squareLength);
		squares[3] = v2(0,squareLength);
		this->scale = scale;
		assert(squares[0].x == 0 && squares[0].y == 0);
	}
Direction getDirection(movment mov){
	if(mov.directionX == 1 && mov.directionY == 0) return Direction::right;
	if(mov.directionX == -1 && mov.directionY == 0) return Direction::left;
	if(mov.directionX == 0 && mov.directionY == 1) return Direction::up;
	if(mov.directionX == 0 && mov.directionY == -1) return Direction::down;
	if(mov.directionX == 1 && mov.directionY == 1) return Direction::diagonalUp;
	if(mov.directionX == -1 && mov.directionY == -1) return Direction::diagonalDown;
	if(mov.directionX == 0 && mov.directionY == 0) return Direction::idle;
	throwErr("Invalid direction");
	return Direction::idle;
}

void doSomethingToSquares(const std::function <void (v2&)>& f){
	for(int i = 0;i < 4; i++){
		f(squares[i]);
	}
}
void translateSquares(float x, float y){
		doSomethingToSquares([x,y](v2 &v){ v = v2(v.x + x,v.y + y);});
}

void rotateSquares(v2 point,float angle){
		doSomethingToSquares([point,angle](v2 &v){
			v.rotate2dPoint(point,angle);
		});
	}
void shearSquares(float xShear, float yShear){
		doSomethingToSquares([xShear,yShear](v2 &v){
			v.shear(xShear, yShear);
		});
	}

void roundSquares(){
		doSomethingToSquares([](v2 &v){v = v2(round(v.x),round(v.y));});
}
void printSquare(){
		cout << squares[0] << " "<< squares[1] << " "<< squares[2] << " "<< squares[3] << " " << endl;
}
void writeCords(TTF_Font *f){
		doSomethingToSquares([f,this](v2 &v){
				v2 cords = Utils::normalize_scale(v,scale);
				const string write = to_string(v.x).substr(0,4) + ", " + to_string(v.y).substr(0,4);
				c->renderText(f,write,cords.x,cords.y,{0,0,0});
		});
}
void setSquares(v2 p1,v2 p2, v2 p3, v2 p4){
		squares[0] = p1;
		squares[1] = p2;
		squares[2] = p3;
		squares[3] = p4;
}
void drawSquare(){
		// cout << squares[0] << endl;
		v2 p1 = Utils::normalize_scale(squares[0],scale);
		v2 p2 = Utils::normalize_scale(squares[1],scale);
		v2 p3 = Utils::normalize_scale(squares[2],scale);
		v2 p4 = Utils::normalize_scale(squares[3],scale);
		// cout << p1 << endl;
		c->draw_line(p1,p2,1,BLACK);
		c->draw_line(p2,p3,1,BLACK);
		c->draw_line(p3,p4,1,BLACK);
		c->draw_line(p4,p1,1,BLACK);
		c->draw_circle(p1, 2,1, BLUE,BLUE);
		c->draw_circle(p2, 2,1, BLUE,BLUE);
		c->draw_circle(p3, 2,1, BLUE,BLUE);
		c->draw_circle(p4, 2,1, BLUE,BLUE);
}
void setMovment(Direction d){
		if(d == Direction::right){
			mov.directionX = 1;
			mov.directionY = 0;
		}
		if(d == Direction::left){
			mov.directionX = -1;
			mov.directionY = 0;
		}
		if(d == Direction::up){
			mov.directionX = 0;
			mov.directionY = 1;
		}
		if(d == Direction::down){
			mov.directionX = 0;
			mov.directionY = -1;
		}
		if(d == Direction::diagonalUp){
			mov.directionX = 1;
			mov.directionY = 1;
		}
		if(d == Direction::diagonalDown){
			mov.directionX = -1;
			mov.directionY = -1;
		}
		if(d == Direction::idle){
			mov.directionX = 0;
			mov.directionY = 0;
		}
	}
void setScalingDirection(ScaledDirection sd){
		scaleDir = sd;
	}
void setShearingDirection(ShearDirection sd){
		shearDir = sd;
	}
void animateTranslation(float dt)
{
		float translateAmount = dt*8;
		if(mov.directionX == -1 && squares[0].x - translateAmount <= -9.0){
				cout << "left boreder ";
				mov.directionX = 0;
				mov.directionY = -1;
				float toTranslate = -9.f - squares[0].x ;
				cout << "to translate " << toTranslate << endl;
				translateSquares( toTranslate, 0);
				roundSquares();
				printSquare();
		}
		else if(mov.directionY == -1 && squares[0].y - translateAmount <= -9.0){
				cout << "down boreder " << endl;
				mov.directionX = 1;
				mov.directionY = 0;
				float toTranslate = -9.f - squares[0].y   ;
				printSquare();
				translateSquares( 0, toTranslate);
				cout << "To translate"  << toTranslate << endl;
				roundSquares();
				printSquare();
		}
		else if(mov.directionX == 1 && squares[2].x + translateAmount >= 9.0){
				mov.directionX = 0;
				mov.directionY = 1;
				float toTranslate = 9.f - squares[2].x;
				translateSquares( toTranslate, 0);
				roundSquares();
		}
		else if(mov.directionY == 1 && squares[2].y + translateAmount >= 9.0){
				mov.directionX = -1;
				mov.directionY = 0;
				float toTranslate = 9.f - squares[2].y;
				translateSquares( 0 , toTranslate);
				roundSquares();
		}
		else {
				translateSquares( translateAmount *(float) mov.directionX,
			   translateAmount * (float) mov.directionY);
		}
		drawSquare();
	}

	void animateRotation(float dt){
		v2 center = v2{squares[0].x + length/2, squares[0].y + length/2};
		rotateSquares(v2{0,0}, 0.73533 * dt);
		drawSquare();
	}
	void animateScaling(float dt){
		// scale=current scale+(scalefactorpersecond×dt)
		if(length >= 18) scaleDir = ScaledDirection::shrink;
		if(length < 1) scaleDir = ScaledDirection::expand;
		ScaledDirection sd = scaleDir;
		doSomethingToSquares([sd](v2 &v){
			if(sd == ScaledDirection::expand) v = v * 1.01;
			if(sd == ScaledDirection::shrink) v = v * 0.99;

		});
		if(sd == ScaledDirection::expand) length = length * 1.01;
		if(sd == ScaledDirection::shrink) length = length * 0.99;
		drawSquare();
	}
	void animateShearing(float dt,ShearAnimation sa){
		if(sa == ShearAnimation::x){
		 	if(squares[2].y >= 4) 
				shearDir = ShearDirection::negative;
			if(squares[1].y <= -2)	
				shearDir =	ShearDirection::positive;
		}
		if(sa == ShearAnimation::y){
		 	if(squares[2].x >= 4) 
				shearDir = ShearDirection::negative;
			if(squares[3].x <= -2)	
				shearDir =	ShearDirection::positive;
		}
		float x = dt;
		float y = dt;
		if(shearDir == ShearDirection::negative){
			x = -x;
			y = -y;
		}
		if(sa == ShearAnimation::x)
			shearSquares(x, 0);
		if(sa == ShearAnimation::y)
			shearSquares(0, y);
		if(sa == ShearAnimation::both)
			shearSquares(x, y);
		drawSquare();
	}

};
