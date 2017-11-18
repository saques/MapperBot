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
    Position(Position &p): x(p.x), y(p.y) {}
    ~Position(){}
		float getX();
		float getY();
		void update(float heading,float delta, int dir);
		static float toDegrees(float rads);
    static Position* applyDelta(Position& p, float delta, float heading);
    static bool headingInRange(float h1, float h2, float EPSILON);
	private:
		float x=0;
		float y=0;
};

#endif
