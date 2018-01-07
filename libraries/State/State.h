#ifndef STATE_H
#define STATE_H
#define DEFAULT_LIMIT 30

#if ARDUINO >= 100
  #include "Arduino.h"
#else
  #include "WProgram.h"
#endif

#include "Motor.h"
#include "HC_SR04.h"
#include "Singleton.h"
#include "Position.h"
#include "Environment.h"

enum Quadrant {N_E, N_W, S_E, S_W};

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
};

class StraightLine: public State {
  public:
    State& act();
    void changeLimit(int newLimit);
  private:
    int limit = DEFAULT_LIMIT;
};

class TriggerRotation: public State {
  public:
    State& act();
};

class ControlRotation: public State {
  public:
    State& act();
    float getTarget();
    void setInitial(float initial);
    void clearTarget();
    void shuffleQuadrants();
  private:
    float target, initial, targetDistance;
    Quadrant quadrants[4] = {N_E, N_W, S_E, S_W};
    void verifyTarget();
    void checkBoundariesAndUpdate(float lower, float upper);
};

class TriggerRotationToTarget: public State {
  public:
    State& act();
};

class ControlRotationToTarget: public State {
  public:
    State& act();
};


/*
* TEST STATES
*/
class TurnOnLedState: public State{
  public:
    State& act();
};

class TurnOffLedState: public State{
  public:
    State& act();
};


#endif
