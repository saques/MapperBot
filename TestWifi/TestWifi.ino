#include <SoftwareSerial.h>
#include <SerialESP8266wifi.h>
#include <HSM5H.h>

#define sw_serial_rx_pin 12 //  Connect this pin to TX on the esp8266
#define sw_serial_tx_pin 13 //  Connect this pin to RX on the esp8266
#define esp8266_reset_pin 8 // Connect this pin to CH_PD on the esp8266, not reset. (let reset be unconnected)
#define PinA_ROT_ENC 2
#define PinB_ROT_ENC 3

void inc(){
  Serial.println("inc");
}

void dec(){
  Serial.println("dec");
}

String inputString;
boolean stringComplete = false;
//SoftwareSerial swSerial(sw_serial_rx_pin, sw_serial_tx_pin);
SerialESP8266wifi wifi(Serial, Serial, esp8266_reset_pin);
char buf[20];
int counter=0;

void setup() {
  inputString.reserve(20);
  //swSerial.begin(9600);
  Serial.begin(9600);

  //initRotaryEncoder(PinA_ROT_ENC, PinB_ROT_ENC, inc, dec);


  wifi.setTransportToUDP();

  wifi.endSendWithNewline(true); // Will end all transmissions with a newline and carrage return ie println.. default is true

  wifi.begin();


  wifi.connectToAP("Retutatario", "123Liguai456");
  wifi.connectToServer("192.168.1.110", "9999");
  wifi.send(SERVER, "ESP8266 test app started");
}

void loop() {
 
  //Send what you typed in the arduino console to the server
  /*
  static char buf[20];
  if (stringComplete) {
    inputString.toCharArray(buf, sizeof buf);
    wifi.send(SERVER, buf);
    inputString = "";
    stringComplete = false;
  }
  */
  

  
  sprintf(buf,"%d",counter);
  wifi.send(SERVER,buf);
  delay(1000);
  counter++;
  
  /*
  WifiMessage in = wifi.listenForIncomingMessage(6000);
  if (in.hasData) {
    //Echo back;
    wifi.send(in.channel, in.message);
  }
  */

}


//Listen for serial input from the console
/*
void serialEvent() {
  while (Serial.available()) {
    char inChar = (char)Serial.read();
    inputString += inChar;
    if (inChar == '\n') {
      stringComplete = true;
    }
  }
}
*/



