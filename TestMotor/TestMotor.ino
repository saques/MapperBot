#include "Motor.h"


//Motor m(6,5,100); //Right motor
Motor m(10,9,100); //Left motor

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  m.forward();
  delay(2000);
  m.backward();
  delay(2000);
}
