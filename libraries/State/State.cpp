#include "State.h"
#include <stdlib.h>

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
  ControlRotation &cr = Singleton<ControlRotation>::getInstance();
  cr.setInitial(heading);
  cr.clearTarget();
  cr.shuffleQuadrants();
  return Singleton<ControlRotation>::getInstance();
}

//ControlRotation
State& ControlRotation::act(){

  Position * objectPosition = Position::applyDelta(*position,distance,heading);

  //Log objectPosition to sd card if necessary
  if(distance < HC_SR04_MAX_RANGE){
    objectPosition->print();
  }
  delete objectPosition;

  //Check if target needs to be updated
  verifyTarget();

  //Exit if 360 degree sweep has been completed
  if(!Position::headingInRange(heading,initial,EPSILON))
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
  if(heading>=lower && heading<upper && targetDistance < distance)
    target = heading;
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
