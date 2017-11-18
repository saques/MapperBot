#include "Position.h"

void Position::update(float heading, float delta, int dir){
	x += cos(heading)*delta*dir;
	y += sin(heading)*delta*dir;
}

float Position::toDegrees(float rads){
  return rads * 180/M_PI;
}

bool Position::headingInRange(float h1, float h2, float EPSILON){
  return abs(h1-h2) <= EPSILON ? true : false;
}

Position* Position::applyDelta(Position& p, float delta, float heading){
  Position* ans = new Position(p);
  ans->x += cos(heading)*delta;
  ans->y += sin(heading)*delta;
  return ans;
}

Position::Position(){}

float Position::getX(){
	return x;
}

float Position::getY(){
	return y;
}
