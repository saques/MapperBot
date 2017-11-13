#ifndef HC_SR04_H
#define HC_SR04_H


#if ARDUINO >= 100
  #include "Arduino.h"
#else
  #include "WProgram.h"
#endif


class HC_SR04 {
	public:
		HC_SR04(int trig, int echo);
		float read();
	private:
		int Trig, Echo;
};


#endif