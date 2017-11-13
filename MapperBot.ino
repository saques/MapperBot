#include <Wire.h>
#include <QMC5883L.h>
#include <HC_SR04.h>

#define BUENOS_AIRES_DEC -0.13962634f
#define Trig 7
#define Echo 8


//QMC5883L compass;
//HC_SR04 ultrasonic(Trig, Echo);

void setup() {
  //Wire.begin();
  Serial.begin(9600);
  //compass.init();
}

void loop() {
  Serial.println(ultrasonic.read());
  /*
  Serial.println(toDegrees(heading()));
  delay(100);
  */
}

float heading(){
  int x,y,z;
  compass.read(&x,&y,&z);

  // Calculate heading when the magnetometer is level, then correct for signs of axis.
  // Atan2() automatically check the correct formula taking care of the quadrant you are in
  float heading = atan2(y, x);

  heading += BUENOS_AIRES_DEC;
  // Find yours here: http://www.magnetic-declination.com/

  // Correct for when signs are reversed.
  if(heading < 0)
    heading += 2*PI;

  // Check for wrap due to addition of declination.
  if(heading > 2*PI)
    heading -= 2*PI;

  return heading;
}

float toDegrees(float deg){
  return deg * 180/M_PI;
}

