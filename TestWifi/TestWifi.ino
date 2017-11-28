#include <SoftwareSerial.h>
#include <SerialESP8266wifi.h>

#define sw_serial_rx_pin 4 //  Connect this pin to TX on the esp8266
#define sw_serial_tx_pin 6 //  Connect this pin to RX on the esp8266
#define esp8266_reset_pin 5 // Connect this pin to CH_PD on the esp8266, not reset. (let reset be unconnected)

SoftwareSerial swSerial(sw_serial_rx_pin, sw_serial_tx_pin);

// the last parameter sets the local echo option for the ESP8266 module..
SerialESP8266wifi wifi(swSerial, swSerial, esp8266_reset_pin);//adding Serial enabled local echo and wifi debug

String inputString;
boolean stringComplete = false;

void setup() {
  inputString.reserve(20);
  swSerial.begin(115200);
  Serial.begin(9600);
  
  while (!Serial)
    ;
  Serial.println("Starting wifi");

  wifi.setTransportToTCP();// this is also default

  wifi.endSendWithNewline(true); // Will end all transmissions with a newline and carrage return ie println.. default is true

  wifi.begin();


  wifi.connectToAP("Retutatario", "");
  wifi.connectToServer("192.168.1.101", "2121");
  Serial.println("Connected");
  wifi.send(SERVER, "ESP8266 test app started");
}

void loop() {

  //Make sure the esp8266 is started..
  
  if (!wifi.isStarted())
    wifi.begin();

  
  //Send what you typed in the arduino console to the server
  
  
  
  
  //Listen for incoming messages and echo back, will wait until a message is received, or max 6000ms..
  WifiMessage in = wifi.getIncomingMessage();
  if (in.hasData) {
    Serial.println(in.message);
    //wifi.send(in.channel, "Echo:", false);
    wifi.send(in.channel, in.message);
  }
  

  //If you want do disconnect from the server use:
  // wifi.disconnectFromServer();

}


