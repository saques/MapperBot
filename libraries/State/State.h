#ifndef STATE_H
#define STATE_H

#if ARDUINO >= 100
  #include "Arduino.h"
#else
  #include "WProgram.h"
#endif

#include "Motor.h"

#include "HC_SR04.h"
#include "Singleton.h"

#define DEFAULT_LIMIT 30



/*
** Interface for declaring States of
** the MapperBot. This should be done
** in such way that the loop() function
** generally looks like this:
**
** void loop() {
**  state = state.act();
** }
**
** As the States will repeat throughout
** all the mapping, the idea is that all
** implementing classes implement the
** Singleton pattern.
*/
class State {
  public:
    virtual State& act() = 0;
    void updateDistance(float d){
      distance = d;
    }
    void updateHeading(float h){
      heading = h;
    }
  protected:
    float distance, heading;
};


class StraightLine: public State {
  public:
    State& act();
    void changeLimit(int newLimit);
  private:
    int limit = DEFAULT_LIMIT;
};

template class Singleton<StraightLine>;

class TriggerRotation: public State {
  public:
    State& act();
};

template class Singleton<TriggerRotation>;

class ControlRotation: public State {
  public:
    State& act();
    float getTarget();
  private:
    float target;
};

template class Singleton<ControlRotation>;

class TriggerRotationToTarget: public State {
  public:
    State& act();
};

template class Singleton<TriggerRotationToTarget>;

class ControlRotationToTarget: public State {
  public:
    State& act();
};

template class Singleton<ControlRotationToTarget>;

#endif
