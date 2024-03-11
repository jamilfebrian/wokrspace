#include "BluetoothSerial.h"
BluetoothSerial SerialBT;

String MACadd = "08:B6:1F:3B:5E:5E";
uint8_t address[6]  = {0x08, 0xB6, 0x1F, 0x3B, 0x5E, 0x5E};
String name = "OBDII";
char *pin = "1234"; //<- standard pin would be provided by default
bool connected;

const int button = 15;

void setup() {
  pinMode(button, INPUT_PULLUP);
  Serial.begin(115200);
  SerialBT.begin("ESP32test", true); 
  Serial.println("The device started in master mode, make sure remote BT device is on!");
 
//   connected = SerialBT.connect(name);
  connected = SerialBT.connect(address);
  
  if(connected) {
    Serial.println("Connected Succesfully!");
  } else {
    while(!SerialBT.connected(10000)) {
      Serial.println("Failed to connect. Make sure remote device is available and in range, then restart app."); 
    }
  }

  if (SerialBT.disconnect()) {
    Serial.println("Disconnected Succesfully!");
  }
  SerialBT.connect();
}

void loop() {
  bool value = digitalRead(button);
  if(!value){
    SerialBT.write('1');
  } else {
    SerialBT.write('0');
  }
  delay(20);
}
