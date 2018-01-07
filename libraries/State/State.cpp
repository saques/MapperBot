#include "State.h"
#include <stdlib.h>

#define EPSILON .0349f //2 degrees

//StraightLine
State& StraightLine::act(){
  if(Environment::getInstance().distance()>limit)
    return *this;
  return Singleton<TriggerRotation>::getInstance();
}

void StraightLine::changeLimit(int newLimit){
  limit = newLimit;
}

//TriggerRotation
State& TriggerRotation::act(){
  Environment& env = Environment::getInstance();

  Motor::Rotate(*env.LMotor(),*env.RMotor(), 1);
  ControlRotation &cr = Singleton<ControlRotation>::getInstance();
  cr.setInitial(env.heading());
  cr.clearTarget();
  cr.shuffleQuadrants();
  return Singleton<ControlRotation>::getInstance();
}

//ControlRotation
State& ControlRotation::act(){
  Environment& env = Environment::getInstance();

  Position * objectPosition = Position::applyDelta(*env.position(),
                                                    env.distance(),
                                                    env.heading());
  //Log objectPosition to sd card if necessary
  if(env.distance() < HC_SR04_MAX_RANGE){
    objectPosition->print();
  }
  delete objectPosition;

  //Check if target needs to be updated
  verifyTarget();

  //Exit if 360 degree sweep has been completed
  if(!Position::headingInRange(env.heading(),initial,EPSILON))
    return *this;

  return Singleton<TriggerRotationToTarget>::getInstance();
}

float ControlRotation::getTarget(){
  return target;
}

void ControlRotation::setInitial(float i){
  initial = i;
}

void ControlRotation::clearTarget(){
  target = targetDistance = -1;
}

void ControlRotation::shuffleQuadrants(){
  for(int i=0; i<4; i++){
    int r = rand()%4;
    Quadrant tmp = quadrants[i];
    quadrants[i] = quadrants[r];
    quadrants[r] = tmp;
  }
}


void ControlRotation::checkBoundariesAndUpdate(float lower, float upper){
  Environment& env = Environment::getInstance();
  if(env.heading()>=lower && env.heading()<upper && targetDistance < env.distance())
    target = env.heading();
}

void ControlRotation::verifyTarget(){
  switch (quadrants[0]) {
    case N_E:
      checkBoundariesAndUpdate(0,M_PI/2);
      break;
    case N_W:
      checkBoundariesAndUpdate(M_PI/2,M_PI);
      break;
    case S_W:
      checkBoundariesAndUpdate(M_PI,3*M_PI/2);
      break;
    case S_E:
      checkBoundariesAndUpdate(3*M_PI/2,2*M_PI);
      break;
    default:
      break;
  }
}

//TriggerRotationToTarget
State& TriggerRotationToTarget::act(){
  Environment& env = Environment::getInstance();
  float target = Singleton<ControlRotation>::getInstance().getTarget();
  float lim = env.heading() > M_PI ? env.heading() - M_PI : env.heading() + M_PI;
  /*
  ** Remember: m1(a) is the left motor and m2(b) is the right one
  */
  if(target<lim){
    Motor::Rotate(*env.LMotor(),*env.RMotor(),1);
  } else {
    Motor::Rotate(*env.LMotor(),*env.RMotor(),-1);
  }
  return Singleton<ControlRotationToTarget>::getInstance();
}

//ControlRotationToTarget
State& ControlRotationToTarget::act(){
  Environment& env = Environment::getInstance();
  float o = Singleton<ControlRotation>::getInstance().getTarget();
  if(!Position::headingInRange(env.heading(),o,EPSILON))
    return *this;
  return Singleton<StraightLine>::getInstance();
}

/*
* TEST STATES
*/
State& TurnOnLedState::act(){
  Serial.println("on");
  digitalWrite(LED_BUILTIN, HIGH);
  delay(500);
  return Singleton<TurnOffLedState>::getInstance();
}

State& TurnOffLedState::act(){
  Serial.println("off");
  digitalWrite(LED_BUILTIN, LOW);
  delay(500);
  return Singleton<TurnOnLedState>::getInstance();
}
