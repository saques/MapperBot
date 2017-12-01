#include <SoftwareSerial.h>
#include <SerialESP8266wifi.h>

#define sw_serial_rx_pin 12 //  Connect this pin to TX on the esp8266
#define sw_serial_tx_pin 13 //  Connect this pin to RX on the esp8266
#define esp8266_reset_pin 8 // Connect this pin to CH_PD on the esp8266, not reset. (let reset be unconnected)

SoftwareSerial swSerial(sw_serial_rx_pin, sw_serial_tx_pin);

// the last parameter sets the local echo option for the ESP8266 module..
SerialESP8266wifi wifi(swSerial, swSerial, esp8266_reset_pin);//adding Serial enabled local echo and wifi debug

String inputString;
boolean stringComplete = false;

void setup() {
  inputString.reserve(20);
  swSerial.begin(9600);
  Serial.begin(9600);
  
  Serial.println("Starting wifi");
 

  wifi.setTransportToTCP();

  wifi.endSendWithNewline(false); // Will end all transmissions with a newline and carrage return ie println.. default is true

  wifi.begin();


  wifi.connectToAP("Retutatario", "");
  wifi.connectToServer("192.168.1.110", "9999");
  Serial.println("Connected");
  wifi.send(SERVER, "ESP8266 test app started");
}

void loop() {

  if (!wifi.isStarted())
    wifi.begin();

  
  //Send what you typed in the arduino console to the server
  static char buf[20];
  if (stringComplete) {
    inputString.toCharArray(buf, sizeof buf);
    wifi.send(SERVER, buf);
    inputString = "";
    stringComplete = false;
  }
  

  /*
  wifi.send(SERVER,"Hola!\r\n");
  delay(1000);
  */

  WifiMessage in = wifi.listenForIncomingMessage(6000);
  if (in.hasData) {
    //Echo back;
    wifi.send(in.channel, in.message);
  }

}


//Listen for serial input from the console
void serialEvent() {
  while (Serial.available()) {
    char inChar = (char)Serial.read();
    inputString += inChar;
    if (inChar == '\n') {
      stringComplete = true;
    }
  }
}



