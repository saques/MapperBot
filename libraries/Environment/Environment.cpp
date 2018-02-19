#include "Environment.h"

void Environment::setWiFi(SerialESP8266wifi * wifi){
  w = wifi;
}

void Environment::setLeftMotor(Motor * m){
  l = m;
}

void Environment::setRightMotor(Motor * m){
  r = r;
}

void Environment::updateHeading(float heading){
  h = heading;
}

void Environment::updateDistance(float distance){
  d = distance;
}

void Environment::updatePosition(Position * p){
  p = p;
}

Position * Environment::position(){
  return p;
}

SerialESP8266wifi * Environment::wifi(){
  return w;
}

Motor * Environment::LMotor(){
  return l;
}

Motor * Environment::RMotor(){
  return r;
}

float Environment::heading(){
  return h;
}

float Environment::distance(){
  return d;
}

void Environment::addCoveredDistance(float covered){
  c += covered;
}

float Environment::coveredDistance(){
  return c;
}

void Environment::resetCoveredDistance(){
  c = 0;
}
