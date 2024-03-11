#include <SPI.h>
#include <MFRC522.h>
#include <LiquidCrystal_I2C.h>
#include <Servo.h>

String UID[3]= {"C4 23 5C 07",
                "47 E2 AF 6B",
                "73 EA D8 1B"};

#define SS_PIN 53
#define RST_PIN 47
#define portal 10
#define echo  9
#define trig  8

const int IR[6] = {2,3,4,5,6,7};
int IRvalue[6];
static String slot="";
unsigned long currTime=0;

MFRC522 mfrc522(SS_PIN, RST_PIN); 
LiquidCrystal_I2C lcd(0x27,16,2); 
Servo servo;

int x=0;
bool statusMasuk, statusOpen; 

void DisplayLCD(int angka){
  switch(angka){
    case 0:
      lcd.setCursor(0,0); lcd.print("   Slot Parkir  ");
      lcd.setCursor(2,1); lcd.print(slot); lcd.print("                 ");
      break; 
    case 1:
        lcd.setCursor(0,0); lcd.print("  Sistem Parkir ");
        lcd.setCursor(0,1); lcd.print("    Otomatis    ");
        delay(1500);
      break;
    case 2:
      lcd.setCursor(0,0); lcd.print(" Silahkan Masuk ");
      lcd.setCursor(0,1); lcd.print("                ");
    break;
    case 3:
      lcd.setCursor(0,0); lcd.print("MAAF! ID Anda   ");
      lcd.setCursor(0,1); lcd.print("Tidak Terdaftar ");
    break;
  }
}

long distance(int TRIG, int ECHO){
  pinMode(TRIG, OUTPUT);    pinMode(ECHO, INPUT); 
  digitalWrite(TRIG, LOW);  delayMicroseconds(8);
  digitalWrite(TRIG, HIGH); delayMicroseconds(8);
  digitalWrite(TRIG, LOW);  delayMicroseconds(8);

  long durasi = pulseIn(ECHO, HIGH); 
  long jarak = (durasi / 2) / 29.1;
  delay(5);
  return jarak;
}

void setup() {
  Serial.begin(9600);
  SPI.begin(); mfrc522.PCD_Init();   
  lcd.begin(); lcd.clear(); lcd.backlight();
  servo.attach(portal);
  servo.write(0);

  for(int i=0; i<6; i++){
    pinMode(IR[i], INPUT);
  }

  DisplayLCD(1);
  Serial.println("Approximate your card to the reader...");
  Serial.println();
  
}

void loop() {
  slot="";
  long jarak = distance( trig, echo);
  
  for(int i=0; i<6; i++){
    IRvalue[i] = digitalRead(IR[i]);
    if(IRvalue[i]==1){
      slot += String(i+1)+String(" ");
    }
  }
  DisplayLCD(0);
  
  String content= "";
  if (mfrc522.PICC_IsNewCardPresent() && mfrc522.PICC_ReadCardSerial()){
   for (byte i = 0; i < mfrc522.uid.size; i++){
     content.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));
     content.concat(String(mfrc522.uid.uidByte[i], HEX));
  } Serial.println();
  content.toUpperCase();
  for(byte i=0; i<5; i++){
    if(content.substring(1) == UID[i]){
      statusMasuk=true;
    }
  } if(!statusMasuk){
      Serial.println("AKses Ditolak");
      DisplayLCD(3);
      servo.write(0);
      statusOpen = false;
      delay(2500); lcd.clear();
    } else if (statusMasuk){
      Serial.println("Silahkan Masuk");
      servo.write(90);
      DisplayLCD(2);
      statusOpen = true;
      delay(2500); lcd.clear();
    }
  } statusMasuk=false;

  if(statusOpen && jarak<=2){
    servo.write(0); statusOpen=false;
    delay(2000);
  } else if(!statusOpen && jarak<=2){
    servo.write(90); x=1;
  } else if(!statusOpen && jarak>2 && x==1){
    delay(2000);
    servo.write(0); x=0;
  } else if(statusOpen && jarak>2){
    servo.write(90);
  }
}
