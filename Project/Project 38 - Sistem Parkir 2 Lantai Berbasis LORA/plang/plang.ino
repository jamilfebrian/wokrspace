#include <SPI.h>
#include <LoRa.h>

#define ss 10
#define rst 9
#define dio0 2

#define IR 3

unsigned long previousMillis = 0, previousMillis2 = 0;
bool statusMobil1, statusMobil2;
int slot = 0;

void setup(){
  Serial.begin(9600);
  while (!Serial);

  LoRa.setPins(ss, rst, dio0);
  pinMode(IR, INPUT);

  if (!LoRa.begin(433E6)) {
    Serial.println("LoRa Gagal Dimulai!");
    while (1);
  } LoRa.setSyncWord(0x33);
  Serial.println("LoRa Plang Siap");
}

void loop(){
  int IRValue = digitalRead(IR);
  Serial.println("X");

  if(!IRValue){
    if (millis() - previousMillis >= 2000 && millis() - previousMillis2 >= 3000) {
      previousMillis = millis();
      if(statusMobil1){
        Serial.println("Mengirim Plang1 Siap");
        LoRa.beginPacket();
        LoRa.print("Plang1"); 
        LoRa.endPacket(true);
        statusMobil1 = false;;
      } else if(statusMobil2){
        Serial.println("Mengirim Plang2 Siap");
        LoRa.beginPacket();
        LoRa.print("Plang2"); 
        LoRa.endPacket(true);
        statusMobil2 = false;;
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
      if(received == "Mobil1"){
        statusMobil1 = true;
      } else if(received == "Mobil2"){
        statusMobil2 = true;;
      }
    }
   }
 }
}
