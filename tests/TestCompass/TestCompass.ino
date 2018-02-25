#include <Wire.h>
#include <stdlib.h>
#include <Position.h>
#include <QMC5883L.h>
#include <Environment.h>
#define BUENOS_AIRES_DEC -0.13962634f

QMC5883L compass;


void setup() {
  Serial.begin(9600);
  //compass startup
  Wire.begin();
  compass.init();
}


void loop() {
  Environment::getInstance().updateHeading(compass.heading(BUENOS_AIRES_DEC));
  Serial.println(Position::toDegrees(Environment::getInstance().heading()));
}
