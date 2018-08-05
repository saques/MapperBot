#include "Position.h"
#include "Environment.h"
#include "SerialESP8266wifi.h"

char buffer[32];

void Position::update(float heading, float delta, int dir){
	x += cos(heading)*delta*dir;
	y += sin(heading)*delta*dir;
}

float Position::toDegrees(float rads){
  return rads * 180/M_PI;
}

float Position::computeHeading(float currHeading, float delta){
	float ans = currHeading + delta;
	return ans > 2*M_PI ? ans - 2*M_PI : (ans < 0 ? ans + 2*M_PI : ans);
}

bool Position::headingInRange(float h1, float h2, float EPSILON){
	float diff = abs(h1-h2);
	if(diff > M_PI)
		diff = abs(2*M_PI-diff);
  return diff <= EPSILON ? true : false;
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

void Position::print(char * header){
	sprintf(buffer, "%s:%s;%s", header, String(x).c_str(), String(y).c_str());
	Environment::getInstance().wifi()->send(SERVER, buffer);
}
