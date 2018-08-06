#include "Motor.h"
#define PWM_VALUES 256

int sign(int x){
  return (x > 0) ? 1 : ((x < 0) ? -1 : 0);
}

void Motor::Rotate(Motor &a, Motor &b, int w){
  if(w>0){
    b.forward();
    a.backward();
  } else {
    a.forward();
    b.backward();
  }
}

void Motor::Stop(Motor &a, Motor &b){
  a.stop();
  b.stop();
}

void Motor::Forward(Motor &a, Motor &b){
  a.forward();
  b.forward();
}

void Motor::Backward(Motor &a, Motor &b){
  a.backward();
  b.backward();
}

Motor::Motor(int pinA, int pinB, int speed){
  pinMode(pinA, OUTPUT);
  pinMode(pinB, OUTPUT);
  digitalWrite(pinA, LOW);
  digitalWrite(pinB, LOW);
  this->pinA = pinA;
  this->pinB = pinB;
  this->speed = (sign(speed)*speed)%PWM_VALUES;
  this->status = 0;
}

void Motor::stop(){
  digitalWrite(pinA, LOW);
  digitalWrite(pinB, LOW);
  this->status = 0;
}

void Motor::forward(){
  set(speed);
  this->status = 1;
}

void Motor::backward(){
  set(-1*speed);
  this->status = -1;
}

void Motor::pause(){
  digitalWrite(pinA, LOW);
  digitalWrite(pinB, LOW);
}

void Motor::resume(){
  if(this->status > 0){
    forward();
  } else if (this->status < 0){
    backward();
  }
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
