#include "Position.h"


float Position::toDegrees(float rads){
  return rads * 180/M_PI;
}

Position::Position(){}

float Position::getX(){
	return x;
}

float Position::getY(){
	return y;
}

void Position::update(float heading, float delta, int dir){
	x += cos(heading)*delta*dir;
	y += sin(heading)*delta*dir;
}