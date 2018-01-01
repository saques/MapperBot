#include <Wire.h>
#include <time.h>
#include <stdlib.h>
#include <Position.h>
#include <QMC5883L.h>
#include <HSM5H.h>
#include <Environment.h>

#include <SoftwareSerial.h>
#include <SerialESP8266wifi.h>
#define sw_serial_rx_pin 12 //  Connect this pin to TX on the esp8266
#define sw_serial_tx_pin 13 //  Connect this pin to RX on the esp8266
#define esp8266_reset_pin 8 // Connect this pin to CH_PD on the esp8266, not reset. (let reset be unconnected)


#define PinA_ROT_ENC 2
#define PinB_ROT_ENC 3
#define BUENOS_AIRES_DEC -0.13962634f
#define DELTA_CIRCUMFERENCE 2.0f

QMC5883L compass;
Position position;
SoftwareSerial swSerial(sw_serial_rx_pin, sw_serial_tx_pin);
SerialESP8266wifi wifi(swSerial, swSerial, esp8266_reset_pin);//adding Serial enabled local echo and wifi debug
char buffer[50];

/*
 * Functrions for rotary encoder, 
 * to be triggered when interrupt
 * fired.
 */
void inc(){
  Serial.print("inc; ");
  Serial.println(compass.getPreviousReading());
  position.update(compass.getPreviousReading(), DELTA_CIRCUMFERENCE, 1);
}

void dec(){
  Serial.print("dec; ");
  Serial.println(compass.getPreviousReading());
  position.update(compass.getPreviousReading(), DELTA_CIRCUMFERENCE, -1);
}


void setup() {
  Serial.begin(9600);

  //compass startup
  Wire.begin();
  compass.init();

  //rotary encoder setup
  initRotaryEncoder(PinA_ROT_ENC, PinB_ROT_ENC, inc, dec);

  //wifi startup
  swSerial.begin(9600);
  Serial.println("Starting wifi");
  wifi.setTransportToUDP();
  wifi.endSendWithNewline(true); // Will end all transmissions with a newline and carrage return ie println.. default is true
  wifi.begin();
  wifi.connectToAP("Retutatario", "");
  wifi.connectToServer("192.168.1.110", "9999");
  wifi.send(SERVER, "ESP8266 test app started");

  

}

/**
 * Some considerations:
 * 
 * I'm assuming that the wifi device will not be transmitting
 * at the same time the robot is moving in a straight line, i.e.,
 * it is not scanning for obstacles. 
 * 
 * This is because both SoftwareSerial and the rotary encoder
 * rely on interrupts to work correctly. 
 * 
 * Perhaps diminishing the baud rate on the ESP8266 may help. Nevertheless,
 * it doesn't seem feasible to transmit the position while the robot is moving
 * because of this issue.
 * 
 * Note that the compass (Wire) also depends on interrupts, but during tests
 * it seemed that the collision is minimal. However, during the scanning process
 * it would be reasonable to allow some time for the compass to update itself, for
 * instance, by diminishing the speed of the motors while rotating in the fixed position.
 */
void loop() {
  Environment::getInstance().updateHeading(compass.heading(BUENOS_AIRES_DEC));
  //Environment::getInstance().updateDistance(ultrasonic.read());
  Environment::getInstance().updatePosition(&position);
  sprintf(buffer,"%d;%d", (int)position.getX(), (int)position.getY());
  delay(2000);
  //detachHSM5H();
  wifi.send(SERVER, buffer);
  //attachHSM5H();
}
