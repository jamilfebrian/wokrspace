#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27,20,4); 

#define IR1Pin 4
#define IR2Pin 3
#define IR3Pin 2
#define IR4Pin 5
#define IR5Pin 6
#define IR6Pin 7

#define LED1  10
#define LED2  12
#define LED3  11
#define LED4  13
#define LED5  A3
#define LED6  A0

void setup() {
  Serial.begin(9600);

  pinMode(IR1Pin, INPUT);
  pinMode(IR2Pin, INPUT);
  pinMode(IR3Pin, INPUT);
  pinMode(IR4Pin, INPUT);
  pinMode(IR5Pin, INPUT);
  pinMode(IR6Pin, INPUT);

  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);
  pinMode(LED3, OUTPUT);
  pinMode(LED4, OUTPUT);
  pinMode(LED5, OUTPUT);
  pinMode(LED6, OUTPUT);
  
  lcd.init(); lcd.backlight(); lcd.clear();

}

void loop() {
  
  bool IR1 = digitalRead(IR1Pin);
  bool IR2 = digitalRead(IR2Pin);
  bool IR3 = digitalRead(IR3Pin);
  bool IR4 = digitalRead(IR4Pin);
  bool IR5 = digitalRead(IR5Pin);
  bool IR6 = digitalRead(IR6Pin); 

  Serial.print(IR1);Serial.print(IR2);
  Serial.print(IR3);Serial.print(IR4);
  Serial.print(IR5);Serial.println(IR6);

  String statusParkir1 = (!IR1) ? "PENUH" : "KOSONG";
  String statusParkir2 = (!IR2) ? "PENUH" : "KOSONG";
  String statusParkir3 = (!IR3) ? "PENUH" : "KOSONG";
  String statusParkir4 = (!IR4) ? "PENUH" : "KOSONG";
  String statusParkir5 = (!IR5) ? "PENUH" : "KOSONG";
  String statusParkir6 = (!IR6) ? "PENUH" : "KOSONG";

  if(IR1){digitalWrite(LED1, HIGH);} else {digitalWrite(LED1, LOW);}
  if(IR2){digitalWrite(LED2, HIGH);} else {digitalWrite(LED2, LOW);}
  if(IR3){digitalWrite(LED3, HIGH);} else {digitalWrite(LED3, LOW);}
  if(IR4){digitalWrite(LED4, HIGH);} else {digitalWrite(LED4, LOW);}
  if(IR5){digitalWrite(LED5, HIGH);} else {digitalWrite(LED5, LOW);}
  if(IR6){digitalWrite(LED6, HIGH);} else {digitalWrite(LED6, LOW);}

  lcd.setCursor(0,0);  lcd.print("     SLOT PARKIR    ");
  lcd.setCursor(0,1);  lcd.print("P1:"); lcd.print(statusParkir1);  lcd.print("  ");
  lcd.setCursor(10,1); lcd.print("P2:"); lcd.print(statusParkir2);  lcd.print("  ");
  lcd.setCursor(0,2);  lcd.print("P3:"); lcd.print(statusParkir3);  lcd.print("  ");
  lcd.setCursor(10,2); lcd.print("P4:"); lcd.print(statusParkir4);  lcd.print("  ");
  lcd.setCursor(0,3);  lcd.print("P5:"); lcd.print(statusParkir5);  lcd.print("  ");
  lcd.setCursor(10,3); lcd.print("P6:"); lcd.print(statusParkir6);  lcd.print("  ");

}
