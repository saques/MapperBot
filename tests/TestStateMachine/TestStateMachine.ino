#include <State.h>

State* s = &Singleton<TurnOnLedState>::getInstance();

void setup() {
  Serial.begin(9600);
  pinMode(LED_BUILTIN, OUTPUT);
}

void loop() {
  s = &s->act();
}
