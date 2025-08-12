#include <SPI.h>
#include <LoRa.h>
#include <LiquidCrystal_I2C.h>

#define ss 10
#define rst 9
#define dio0 2

int slot = 2;
int lastSlot = -1;
unsigned long previousMillis = 0;

LiquidCrystal_I2C lcd(0x27,20,4); 

void setup() {
  Serial.begin(9600);
  while (!Serial);

  lcd.init(); lcd.backlight(); lcd.clear();

  lcd.setCursor(0,1);
  lcd.print(" Jumlah Slot Parkir ");
  lcd.setCursor(0,2);
  lcd.print("    Tersedia : 2  ");

  LoRa.setPins(ss, rst, dio0);
  
  if (!LoRa.begin(433E6)) {
    Serial.println("LoRa Gagal Dimulai!");
    while (1);
  } LoRa.setSyncWord(0x33);
  Serial.println("LoRa LCD Siap");
}

void loop() {
  Serial.println("X");
  
    int packetSize = LoRa.parsePacket();
    
    if (packetSize) {
      String received = "";
      while (LoRa.available()) {
        received += (char)LoRa.read();
      }
  
      Serial.print("Diterima: ");
      Serial.println(received);

      if(received == "Mobil1" || received == "Mobil2" ){
        slot--;
      } else if(received == "Plang1" || received == "Plang2"){
        slot++;
      }
    }
    
 if(slot != lastSlot){
  if(slot == 2){
      lcd.setCursor(0,1);
      lcd.print(" Jumlah Slot Parkir ");
      lcd.setCursor(0,2);
      lcd.print("    Tersedia : 2  ");
    } else if(slot == 1){
      lcd.setCursor(0,1);
      lcd.print(" Jumlah Slot Parkir ");
      lcd.setCursor(0,2);
      lcd.print("    Tersedia : 1  ");
    } else if(slot == 0){
      lcd.setCursor(0,1);
      lcd.print(" Jumlah Slot Parkir ");
      lcd.setCursor(0,2);
      lcd.print("       PENUH        ");
    } else if(slot > 2){
      slot = 2;
    } else if(slot < 0){
      slot = 0;
    } lastSlot = slot;
    delay(2000)
 }
}
