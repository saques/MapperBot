#include <Wire.h>
#include <time.h>
#include <stdlib.h>
#include <SoftwareSerial.h>
#include <SerialESP8266wifi.h>
#include <Position.h>
#include <HC_SR04.h>
#include <HSM5H.h>
#include <State.h>
#include <Motor.h>
#include <Environment.h>
//wifi
#define sw_serial_rx_pin A0
#define sw_serial_tx_pin A1
#define esp8266_reset_pin 13
//Position
#define DELTA_CIRCUMFERENCE 2.04203522484f
#define AXIS_LENGTH 18.5f
//Ultrasonic Sensor
#define Trig 7
#define Echo 8
//Rotary Encoder
#define PinA_ROT_ENC 2
#define PinB_ROT_ENC 3
//Motors
#define PIN_A_l 10
#define PIN_B_l 5
#define PIN_A_r 6
#define PIN_B_r 9
#define MOTOR_SPEED 120

Position position;
HC_SR04 ultrasonic(Trig, Echo);
Motor l(PIN_A_l, PIN_B_l, MOTOR_SPEED);
Motor r(PIN_A_r, PIN_B_r, MOTOR_SPEED);
SoftwareSerial swSerial(sw_serial_rx_pin, sw_serial_tx_pin);
SerialESP8266wifi wifi(Serial, Serial, esp8266_reset_pin, swSerial);//adding Serial enabled local echo and wifi debug
State* state = &Singleton<TriggerRotation>::getInstance();
float delta = DELTA_CIRCUMFERENCE/AXIS_LENGTH;

/*
 * Functrions for rotary encoder,
 * to be triggered when interrupt
 * fired.
 */
void inc(){
  Environment& env = Environment::getInstance();
  if(state->rotationState()) {
    env.updateHeading(Position::computeHeading(env.heading(), delta));
    return;
  }
  env.position()->update(env.heading(), DELTA_CIRCUMFERENCE, 1);
  if(state->addToCovered())
    env.addCoveredDistance(DELTA_CIRCUMFERENCE);

}

void dec(){
  Environment& env = Environment::getInstance();
  if(state->rotationState()) {
    env.updateHeading(Position::computeHeading(env.heading(), -1*delta));
    return;
  }
  env.position()->update(env.heading(), DELTA_CIRCUMFERENCE, -1);
}

void setup() {
  Serial.begin(9600);
  swSerial.begin(9600);
  randomSeed(analogRead(2));
  initRotaryEncoder(PinA_ROT_ENC, PinB_ROT_ENC, inc, dec);

  //wifi startup

  
  wifi.setTransportToUDP();
  wifi.endSendWithNewline(true);
  wifi.begin();
  wifi.connectToAP("Retutatario", "");
  wifi.connectToServer("255.255.255.255", "9999");
  wifi.send(SERVER, "MapperBot v1.0");
  
  
  

  Environment::getInstance().setLeftMotor(&l);
  Environment::getInstance().setRightMotor(&r);
  Environment::getInstance().setWiFi(&wifi);
  Environment::getInstance().updatePosition(&position);
  //To prevent overflow in (millis()-TIME_WINDOW)
  delay(2000);
}

void loop() {
  float distance = 0;
  while((distance = ultrasonic.read())<0.01);
  Environment::getInstance().updateDistance(distance);
  state = &state->act();
}
