#ifndef MOTOR_H
#define MOTOR_H

#if ARDUINO >= 100
  #include "Arduino.h"
#else
  #include "WProgram.h"
#endif


class Motor {

	public:

    /*
    ** Positive w: counterclockwise rotation.
    ** Negative w: clockwise rotation.
    ** a is the left Motor
    ** b is the right motor
    */
    static void Rotate(Motor &a, Motor &b, int w);

    static void Stop(Motor &a, Motor &b);

    static void Forward(Motor &a, Motor &b);

    static void Backward(Motor &a, Motor &b);

    /*
    ** pinA and pinB are PWM pins.
    ** speed is a fixed value to be
    ** sent to said PWM pins.
    */
		Motor(int pinA, int pinB, int speed);

    void stop();

    void forward();

    void backward();

    /*
    ** This method receives a value in
    ** [-255,255].
    ** Negative means reverse.
    */
    void set(int speed);

  private:
    int pinA, pinB, speed;

};

#endif
