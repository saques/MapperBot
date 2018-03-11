#ifndef STATE_H
#define STATE_H
#define DEFAULT_LIMIT 30
#define DEFAULT_MAX_COVERED_LIMIT 150

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
    virtual bool addToCovered() {
      return false;
    }
    virtual char * name(){
      return "State";
    }
};

class StraightLine: public State {
  public:
    State& act();
    void changeLimit(int newLimit);
    void changeMaxCoveredLimit(int newLimit);
    bool addToCovered(){
      return true;
    }
    char * name(){
      return "StraightLine";
    }
  private:
    int limit = DEFAULT_LIMIT;
    int maxCoveredLimit = DEFAULT_MAX_COVERED_LIMIT;
};

class TriggerRotation: public State {
  public:
    State& act();
    char * name(){
      return "TriggerRotation";
    }
};

class ControlRotation: public State {
  public:
    State& act();
    float getTarget();
    void setInitial(float initial);
    void reset();
    void shuffleQuadrants();
    void changeLimit(int newLimit);
    void setMaxCoveredTriggered();
    char * name(){
      return "ControlRotation";
    }
  private:
    float target, initial, targetDistance;
    int limit = DEFAULT_LIMIT;
    bool maxCoveredTriggered = false;
    Quadrant quadrants[4] = {N_E, N_W, S_E, S_W};
    void verifyTarget();
    void checkBoundariesAndUpdate(float lower, float upper);
};

class TriggerRotationToTarget: public State {
  public:
    State& act();
    char * name(){
      return "TriggerRotationToTarget";
    }
};

class ControlRotationToTarget: public State {
  public:
    State& act();
    char * name(){
      return "ControlRotationToTarget";
    }
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
