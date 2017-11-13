#include "HSM5H.h"

void pinAInt();
void pinBInt();

void_f inc_f, dec_f;

int pinAVal, pinBVal;

//expecting A or B flags
int expA, expB;
int A,B;

void initRotaryEncoder(int PinA, int PinB, void_f decrement, void_f increment){
  inc_f = increment;
  dec_f = decrement;
  
  A = PinA;
  B = PinB;
  expA = expB = 0;
  
  pinMode(PinA, INPUT_PULLUP);
  pinMode(PinB, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(PinA),pinAInt,RISING);
  attachInterrupt(digitalPinToInterrupt(PinB),pinBInt,RISING);
}

void readPins(){
  pinAVal = digitalRead(A);
  pinBVal = digitalRead(B);
}

void pinAInt(){
  noInterrupts();
  readPins();
  if(pinAVal == HIGH && pinBVal == HIGH && expA){
	inc_f();
    expA = 0;
    expB = 0;
  } else if( pinBVal == LOW && pinAVal == HIGH ) {
    expB = 1;
  }
  interrupts();
}

void pinBInt(){
  noInterrupts();
  readPins();
  if(pinBVal == HIGH && pinAVal == HIGH && expB){
	dec_f();
    expA = 0;
    expB = 0;
  } else if( pinAVal == LOW && pinBVal == HIGH ) {
    expA = 1;
  }
  interrupts();
}

