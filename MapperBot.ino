#include <Wire.h>
#include <time.h>
#include <stdlib.h>

//wifi
#include <SoftwareSerial.h>
#include <SerialESP8266wifi.h>
#define sw_serial_rx_pin 0 //  Connect this pin to TX on the esp8266
#define sw_serial_tx_pin 0 //  Connect this pin to RX on the esp8266
#define esp8266_reset_pin 0 // Connect this pin to CH_PD on the esp8266, not reset. (let reset be unconnected)

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

#include <Environment.h>

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
State* state = &Singleton<StraightLine>::getInstance();

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
  srand(time(NULL));
  Wire.begin();
  Serial.begin(9600);
  compass.init();
  initRotaryEncoder(PinA_ROT_ENC, PinB_ROT_ENC, inc, dec);


  Environment::getInstance().setLeftMotor(&m1);
  Environment::getInstance().setRightMotor(&m2);
  //Environment::getInstance().setWifi(&wifi);
}

void loop() {
  Environment::getInstance().updateHeading(compass.heading(BUENOS_AIRES_DEC));
  Environment::getInstance().updateDistance(ultrasonic.read());
  Environment::getInstance().updatePosition(&position);
  state = &state->act();
}
