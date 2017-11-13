#include <Wire.h>

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

/*
 * Functrions for rotary encoder, 
 * to be triggered when interrupt
 * fired.
 */
int pos = 0;
void inc(){
  pos++;
  Serial.println(pos);
}

void dec(){
  pos--;
  Serial.println(pos);
}


QMC5883L compass;
//HC_SR04 ultrasonic(Trig, Echo);

void setup() {
  Wire.begin();
  Serial.begin(9600);
  compass.init();
  //initRotaryEncoder(PinA_ROT_ENC, PinB_ROT_ENC, inc, dec);
}

void loop() {
  
  //Serial.println(ultrasonic.read());
  Serial.println(toDegrees(compass.heading(BUENOS_AIRES_DEC)));
  delay(100);
  
}


float toDegrees(float deg){
  return deg * 180/M_PI;
}

