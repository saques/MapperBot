#include "Environment.h"

void Environment::setWiFi(SerialESP8266wifi * wifi){
  this->w = wifi;
}

void Environment::setLeftMotor(Motor * m){
  this->l = m;
}

void Environment::setRightMotor(Motor * m){
  this->l = r;
}

void Environment::updateHeading(float heading){
  this->h = heading;
}

void Environment::updateDistance(float distance){
  this->d = distance;
}

void Environment::updatePosition(Position * p){
  this->p = p;
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
