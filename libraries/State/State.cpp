#include "State.h"


//StraightLine
State& StraightLine::act(){
  if(distance>limit){
    return *this;
  }
  return Singleton<TriggerRotation>::getInstance();
}

void StraightLine::changeLimit(int newLimit){
  limit = newLimit;
}

//TriggerRotation
State& TriggerRotation::act(){
  return Singleton<ControlRotation>::getInstance();
}

//ControlRotation
State& ControlRotation::act(){
  return Singleton<TriggerRotationToTarget>::getInstance();
}

float ControlRotation::getTarget(){
  return target;
}

//TriggerRotationToTarget
State& TriggerRotationToTarget::act(){
  return Singleton<ControlRotationToTarget>::getInstance();
}

//ControlRotationToTarget
State& ControlRotationToTarget::act(){
  return Singleton<StraightLine>::getInstance();
}
