#include <Wire.h>

//Position
#include <Position.h>
#define DELTA_CIRCUMFERENCE 2.0f

//Magnetometer
#include <QMC5883L.h>
#define BUENOS_AIRES_DEC -0.13962634f

//Ultrasonic Sensor
#include <HC_SR04.h>
#define Trig 7
#define Echo 8

//Rotary Encoder
#include <HSM5H.h>
#define PinA_ROT_ENC 2
#define PinB_ROT_ENC 3

//State
#include <State.h>

//Motors
#include <Motor.h>
#define PIN_A_1 0
#define PIN_B_1 0
#define PIN_A_2 0
#define PIN_B_2 0
#define MOTOR_SPEED 30

QMC5883L compass;
Position position;
HC_SR04 ultrasonic(Trig, Echo);
Motor m1(PIN_A_1, PIN_B_1, MOTOR_SPEED);
Motor m2(PIN_A_2, PIN_B_2, MOTOR_SPEED);
State& state = Singleton<StraightLine>::getInstance();

/*
 * Functrions for rotary encoder, 
 * to be triggered when interrupt
 * fired.
 */
void inc(){
  position.update(compass.getPreviousReading(), DELTA_CIRCUMFERENCE, 1);
}

void dec(){
  position.update(compass.getPreviousReading(), DELTA_CIRCUMFERENCE, -1);
}

void setup() {
  Wire.begin();
  Serial.begin(9600);
  compass.init();
  initRotaryEncoder(PinA_ROT_ENC, PinB_ROT_ENC, inc, dec);

  Singleton<TriggerRotation>::getInstance().setMotors(&m1,&m2);
  Singleton<ControlRotation>::getInstance().setMotors(&m1,&m2);
  Singleton<TriggerRotationToTarget>::getInstance().setMotors(&m1,&m2);
  Singleton<ControlRotationToTarget>::getInstance().setMotors(&m1,&m2);
  
}

void loop() {
  state.updateHeading(compass.heading(BUENOS_AIRES_DEC));
  state.updateDistance(ultrasonic.read());
  state.updatePosition(&position);
  state = state.act();
}

