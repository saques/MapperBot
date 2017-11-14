#include "Motor.h"
#define PWM_VALUES 256

int sign(int x){
  return (x > 0) ? 1 : ((x < 0) ? -1 : 0);
}

void Motor::Rotate(Motor a, Motor b){
  a.forward();
  b.backward();
}

void Motor::Stop(Motor a, Motor b){
  a.stop();
  b.stop();
}

void Motor::Forward(Motor a, Motor b){
  a.forward();
  b.forward();
}

void Motor::Backward(Motor a, Motor b){
  a.backward();
  b.backward();
}

Motor::Motor(int pinA, int pinB, int speed){
  this->pinA = pinA;
  this->pinB = pinB;
  this->speed = (sign(speed)*speed)%PWM_VALUES;
}

void Motor::stop(){
  digitalWrite(pinA, LOW);
  digitalWrite(pinB, LOW);
}

void Motor::forward(){
  set(speed);
}

void Motor::backward(){
  set(-1*speed);
}

void Motor::set(int s){
  int sg = sign(s);
  s = (sg*s)%PWM_VALUES;
  stop();
  if(sg>0){
    analogWrite(pinB, s);
  } else {
    analogWrite(pinA, s);
  }
}
