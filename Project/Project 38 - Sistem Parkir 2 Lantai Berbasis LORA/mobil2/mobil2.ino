#include <SPI.h>
#include <LoRa.h>
#include <Wire.h>
#include <TFLI2C.h> 

#define ss 10
#define rst 9
#define dio0 2

#define redLamp   3
#define greenLamp 4

TFLI2C tflI2C;
int16_t  tfDist;
int16_t  tfAddr = TFL_DEF_ADR;
String dataLora;

bool statusMobil = false;

unsigned long previousMillis = 0, previousMillis2 = 0;

void setup() {
  Serial.begin(9600);
  while (!Serial);

  pinMode(redLamp, OUTPUT);
  pinMode(greenLamp, OUTPUT);

  digitalWrite(redLamp, HIGH);
  digitalWrite(greenLamp, LOW);

  LoRa.setPins(ss, rst, dio0);

  if (!LoRa.begin(433E6)) { 
    Serial.println("LoRa Gagal Dimulai!");
    while (1);
  } LoRa.setSyncWord(0x33);
  Serial.println("LoRa Sender Siap");
}

void loop() {
//Serial.println("X");
  if(tflI2C.getData(tfDist, tfAddr)){
    
     Serial.println(String(tfDist)+" cm");
     if(tfDist < 150){
      if (millis() - previousMillis >= 2000 && millis() - previousMillis2 >= 2000) {
        previousMillis = millis();
        if(!statusMobil){
          Serial.println("Mengirim Pesan Mobil Siap");
          digitalWrite(redLamp, LOW);
          digitalWrite(greenLamp, HIGH);
          LoRa.beginPacket();
          LoRa.print("Mobil2"); 
          LoRa.endPacket(true);
          statusMobil = true;
        }
      }
     } else {
      if (millis() - previousMillis >= 3000) {
        int packetSize = LoRa.parsePacket();
        previousMillis2 = millis();
        
        if (packetSize) {
          String received = "";
          while (LoRa.available()) {
            received += (char)LoRa.read();
          }
      
          Serial.print("Diterima: ");
          Serial.println(received);
          if(received == "Plang2"){
            digitalWrite(redLamp, HIGH);
            digitalWrite(greenLamp, LOW);
            statusMobil = false;
          }
        }
      }
     }
  }
}
