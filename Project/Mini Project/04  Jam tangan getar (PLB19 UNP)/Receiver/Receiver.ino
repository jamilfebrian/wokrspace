#include "BluetoothSerial.h"

#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
#endif

BluetoothSerial SerialBT;
const int getar = 18;

void setup() {
  pinMode(2,OUTPUT);
  pinMode(getar,OUTPUT);
  Serial.begin(115200);
  SerialBT.begin("ESP32test"); 
  Serial.println("The device started, now you can pair it with bluetooth!");
}

void loop() {
  if (SerialBT.available()) {
    digitalWrite(2, HIGH);
    char value = SerialBT.read();
    Serial.println(value);
    if(value=='1'){
      digitalWrite(getar,HIGH);
    } else if (value=='0'){
      digitalWrite(getar,LOW);
    }
  } else {
     digitalWrite(2, LOW);
  }
  delay(20);
}
