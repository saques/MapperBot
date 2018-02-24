#include "State.h"
#include <stdlib.h>

#define EPSILON .0349f //2 degrees
#define EPSILON_MULTIPLIER 5

//StraightLine
State& StraightLine::act(){
  Environment& env = Environment::getInstance();
  Motor::Forward(*env.LMotor(),*env.RMotor());
  if(env.distance() < limit){
    env.resetCoveredDistance();
    return Singleton<TriggerRotation>::getInstance();
  }
  if(env.coveredDistance() > maxCoveredLimit){
    ControlRotation &cr = Singleton<ControlRotation>::getInstance();
    cr.setMaxCoveredTriggered();
    env.resetCoveredDistance();
    return Singleton<TriggerRotation>::getInstance();
  }
  return *this;
}

void StraightLine::changeLimit(int newLimit){
  limit = newLimit;
}

void StraightLine::changeMaxCoveredLimit(int newLimit){
  maxCoveredLimit = newLimit;
}




//TriggerRotation
State& TriggerRotation::act(){
  Environment& env = Environment::getInstance();

  Motor::Rotate(*env.LMotor(),*env.RMotor(), 1);
  ControlRotation &cr = Singleton<ControlRotation>::getInstance();
  cr.setInitial(env.heading());
  cr.shuffleQuadrants();
  return Singleton<ControlRotation>::getInstance();
}




//ControlRotation
State& ControlRotation::act(){
  Environment& env = Environment::getInstance();

  //Send objectPosition if needed
  if(env.distance() < HC_SR04_MAX_RANGE){
    Position * objectPosition =
      Position::applyDelta(*env.position(), env.distance(), env.heading());
    objectPosition->print();
    delete objectPosition;
  }

  //Check if target needs to be updated
  verifyTarget();

  //Exit if 360 degree sweep has been completed
  if(!Position::headingInRange(env.heading(),initial,EPSILON))
    return *this;

  //Avoid going back in the same direction
  //This observes the case when the bot is in
  //a sort of corridor
  if(maxCoveredTriggered){
    float backwards = initial < M_PI ? initial + M_PI : initial - M_PI;
    if(Position::headingInRange(target, backwards, EPSILON*EPSILON_MULTIPLIER)){
      target = initial;
    }
  }
  return Singleton<TriggerRotationToTarget>::getInstance();
}

float ControlRotation::getTarget(){
  return target;
}

void ControlRotation::setInitial(float i){
  initial = i;
}

void ControlRotation::reset(){
  target = targetDistance = -1;
  maxCoveredTriggered = false;
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
  if(env.heading()>=lower && env.heading()<upper && targetDistance < env.distance()){
    targetDistance = env.distance();
    target = env.heading();
  }
}

void ControlRotation::verifyTarget(){
  Environment& env = Environment::getInstance();

  //Return if too close to walls to get relevant info
  if(env.distance() < limit)
    return;

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

void ControlRotation::changeLimit(int newLimit){
  limit = newLimit;
}

void ControlRotation::setMaxCoveredTriggered(){
  maxCoveredTriggered = true;
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

  //If heading in the right direction, begin to
  //go in a StraightLine
  ControlRotation &cr = Singleton<ControlRotation>::getInstance();
  cr.reset();
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
