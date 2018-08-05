#ifndef HC_SR04_H
#define HC_SR04_H

//In theory is 400, but setting this value
//lower may be safer
#define HC_SR04_MAX_RANGE 390
#define HC_SR04_SAFE_DIVIDER 2

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
