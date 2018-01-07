#include <SoftwareSerial.h>
#include <SerialESP8266wifi.h>

#define sw_serial_rx_pin A0 //  Connect this pin to TX on the esp8266
#define sw_serial_tx_pin A1 //  Connect this pin to RX on the esp8266
#define esp8266_reset_pin 8 // Connect this pin to CH_PD on the esp8266, not reset. (let reset be unconnected)

String inputString;
boolean stringComplete = false;
SoftwareSerial swSerial(sw_serial_rx_pin, sw_serial_tx_pin);
SerialESP8266wifi wifi(Serial, Serial, esp8266_reset_pin, swSerial);
char buf[20];
int counter=0;

void setup() {
  inputString.reserve(20);
  Serial.begin(9600);
  swSerial.begin(9600);

  wifi.setTransportToUDP();

  wifi.endSendWithNewline(true);

  wifi.begin();


  wifi.connectToAP("Retutatario", "");
  wifi.connectToServer("192.168.1.110", "9999");
  wifi.send(SERVER, "ESP8266 test app started");
}

void loop() {
 
  //Send what you typed in the arduino console to the server

  beep();
  
  WifiMessage in = wifi.getIncomingMessage();
  if (in.hasData) {
    //Echo back;
    wifi.send(in.channel, in.message);
  }
  

}

void tick(){
  sprintf(buf,"%d",counter);
  wifi.send(SERVER,buf);
  delay(1000);
  counter++;
}

void beep(){
  if(counter==1000){
    counter=0;
    wifi.send(SERVER,"beep");
  }
  counter++;
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




