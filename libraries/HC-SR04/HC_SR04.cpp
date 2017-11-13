#include "HC_SR04.h"

HC_SR04::HC_SR04(int Trig, int Echo){
	this->Trig = Trig;
	this->Echo = Echo;
	pinMode(Trig, OUTPUT);
	pinMode(Echo, INPUT);
}

float HC_SR04::read(){
  digitalWrite(Trig, LOW);
  delayMicroseconds(2);

  digitalWrite(Trig, HIGH);
  delayMicroseconds(10);

  //340.29 m/s -> .00034029 m/ms -> .034029 cm/ms
  return pulseIn(Echo, HIGH)*.034029 / 2;
  
}