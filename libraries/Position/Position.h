#ifndef POSITION_H
#define POSITION_H

#if ARDUINO >= 100
  #include "Arduino.h"
#else
  #include "WProgram.h"
#endif

class Position {
	
	public:
		Position();
		float getX();
		float getY();
		void update(float heading,float delta, int dir);
		static float toDegrees(float rads);
	private:
		float x=0;
		float y=0;
};

#endif