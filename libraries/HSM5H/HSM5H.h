#ifndef HSM5H_H
#define HSM5H_H

#if ARDUINO >= 100
  #include "Arduino.h"
#else
  #include "WProgram.h"
#endif

typedef void(*void_f)(void);

void initRotaryEncoder(int PinA, int PinB, void_f decrement, void_f increment);

#endif