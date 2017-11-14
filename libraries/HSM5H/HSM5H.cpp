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

void setExp(int A, int B){
	expA = A;
	expB = B;
}

void pinAInt(){
	readPins();
	if(pinAVal == HIGH && pinBVal == HIGH && expA){
		inc_f();
		setExp(0,0);
	} else if( pinBVal == LOW && pinAVal == HIGH ) {
		setExp(0,1);
	}
}

void pinBInt(){
	readPins();
	if(pinBVal == HIGH && pinAVal == HIGH && expB){
		dec_f();
		setExp(0,0);
	} else if( pinAVal == LOW && pinBVal == HIGH ) {
		setExp(1,0);
	}
}

