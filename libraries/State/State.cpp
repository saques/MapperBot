#include "State.h"

#define EPSILON .0349f //2 degrees

//StraightLine
State& StraightLine::act(){
  if(distance>limit)
    return *this;
  return Singleton<TriggerRotation>::getInstance();
}

void StraightLine::changeLimit(int newLimit){
  limit = newLimit;
}

//TriggerRotation
State& TriggerRotation::act(){
  Motor::Rotate(*m1,*m2, 1);
  Singleton<ControlRotation>::getInstance().setInitial(heading);
  return Singleton<ControlRotation>::getInstance();
}

//ControlRotation
State& ControlRotation::act(){

  Position * objectPosition = Position::applyDelta(*position,distance,heading);

  if(distance < HC_SR04_MAX_RANGE){
    //log objectPosition
  }

  //discretize and write distance to array

  delete objectPosition;

  if(!Position::headingInRange(heading,initial,EPSILON))
    return *this;

  //calculate target heading

  return Singleton<TriggerRotationToTarget>::getInstance();
}

float ControlRotation::getTarget(){
  return target;
}

void ControlRotation::setInitial(float i){
  initial = i;
}

//TriggerRotationToTarget
State& TriggerRotationToTarget::act(){
  float target = Singleton<ControlRotation>::getInstance().getTarget();
  float lim = heading > M_PI ? heading - M_PI : heading + M_PI;
  /*
  ** Remember: m1(a) is the left motor and m2(b) is the right one
  */
  if(target<lim){
    Motor::Rotate(*m1,*m2,1);
  } else {
    Motor::Rotate(*m1,*m2,-1);
  }
  return Singleton<ControlRotationToTarget>::getInstance();
}

//ControlRotationToTarget
State& ControlRotationToTarget::act(){
  float o = Singleton<ControlRotation>::getInstance().getTarget();
  if(!Position::headingInRange(heading,o,EPSILON))
    return *this;
  return Singleton<StraightLine>::getInstance();
}
