#include <Wire.h>
#include <time.h>
#include <stdlib.h>
#include <SoftwareSerial.h>
#include <SerialESP8266wifi.h>
#include <Position.h>
#include <QMC5883L.h>
#include <HC_SR04.h>
#include <HSM5H.h>
#include <State.h>
#include <Motor.h>
#include <Environment.h>
//wifi
#define sw_serial_rx_pin A0 
#define sw_serial_tx_pin A1 
#define esp8266_reset_pin 8
//Position
#define DELTA_CIRCUMFERENCE 2.0f
//Magnetometer
#define BUENOS_AIRES_DEC -0.13962634f
//Ultrasonic Sensor
#define Trig 7
#define Echo 8
//Rotary Encoder
#define PinA_ROT_ENC 2
#define PinB_ROT_ENC 3
//Motors
#define PIN_A_l 10
#define PIN_B_l 9
#define PIN_A_r 6
#define PIN_B_r 5
#define MOTOR_SPEED 30

QMC5883L compass;
Position position;
HC_SR04 ultrasonic(Trig, Echo);
Motor l(PIN_A_l, PIN_B_l, MOTOR_SPEED);
Motor r(PIN_A_r, PIN_B_r, MOTOR_SPEED);
SoftwareSerial swSerial(sw_serial_rx_pin, sw_serial_tx_pin);
SerialESP8266wifi wifi(Serial, Serial, esp8266_reset_pin, swSerial);//adding Serial enabled local echo and wifi debug
State* state = &Singleton<StraightLine>::getInstance();

/*
 * Functrions for rotary encoder,
 * to be triggered when interrupt
 * fired.
 */
void inc(){
  position.update(compass.getPreviousReading(), DELTA_CIRCUMFERENCE, 1);
  if(state->addToCovered())
    Environment::getInstance().addCoveredDistance(DELTA_CIRCUMFERENCE);
}

void dec(){
  position.update(compass.getPreviousReading(), DELTA_CIRCUMFERENCE, -1);
}

void setup() {
  srand(time(NULL));
  Serial.begin(9600);
  swSerial.begin(9600);
  Wire.begin();
  compass.init();
  initRotaryEncoder(PinA_ROT_ENC, PinB_ROT_ENC, inc, dec);

  //wifi startup
  wifi.setTransportToUDP(); //Minimizing traffic saves battery (?)
  wifi.endSendWithNewline(true);
  wifi.begin();
  wifi.connectToAP("", "");
  wifi.connectToServer("192.168.1.110", "9999");
  wifi.send(SERVER, "MapperBot v1.0");

  Environment::getInstance().setLeftMotor(&l);
  Environment::getInstance().setRightMotor(&r);
  Environment::getInstance().setWiFi(&wifi);
}

void loop() {
  Environment::getInstance().updateHeading(compass.heading(BUENOS_AIRES_DEC));
  Environment::getInstance().updateDistance(ultrasonic.read());
  Environment::getInstance().updatePosition(&position);
  state = &state->act();
}
