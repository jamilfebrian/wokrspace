#include <Servo.h>
#include <LiquidCrystal_I2C.h>

#define button1 A1
#define button2 A2

#define IR1Pin 2
#define IR2Pin 3
#define IR3Pin 4
#define IR4Pin 5
#define IR5Pin 6
#define IR6Pin 7

#define servoPin1  9
#define servoPin2  8

Servo servoPintuMasuk;
Servo servoPintuKeluar;

LiquidCrystal_I2C lcd(0x27,20,4); 

unsigned long currenTime = 0;
bool statusServoMasuk;
bool statusServoKeluar;

void setup() {
  Serial.begin(115200);
  pinMode(button1, INPUT_PULLUP);
  pinMode(button2, INPUT_PULLUP);

  pinMode(IR1Pin, INPUT);
  pinMode(IR2Pin, INPUT);
  pinMode(IR3Pin, INPUT);
  pinMode(IR4Pin, INPUT);
  pinMode(IR5Pin, INPUT);
  pinMode(IR6Pin, INPUT);

  lcd.init(); lcd.backlight(); lcd.clear();

  servoPintuMasuk.attach(servoPin1); 
  servoPintuKeluar.attach(servoPin2);
  servoPintuMasuk.write(0);
  servoPintuKeluar.write(0);

}

void loop() {
  bool pintuMasuk = digitalRead(button1);
  bool pintuKeluar = digitalRead(button2);

  bool IR1 = digitalRead(IR1Pin);
  bool IR2 = digitalRead(IR2Pin);
  bool IR3 = digitalRead(IR3Pin);
  bool IR4 = digitalRead(IR4Pin);
  bool IR5 = digitalRead(IR5Pin);
  bool IR6 = digitalRead(IR6Pin); 

  Serial.print(pintuMasuk);Serial.print(pintuKeluar);Serial.print(" ");
  Serial.print(IR1);Serial.print(IR2);
  Serial.print(IR3);Serial.print(IR4);
  Serial.print(IR5);Serial.println(IR6);

  String statusParkir1 = (!IR1) ? "PENUH" : "KOSONG";
  String statusParkir2 = (!IR2) ? "PENUH" : "KOSONG";
  String statusParkir3 = (!IR3) ? "PENUH" : "KOSONG";
  String statusParkir4 = (!IR4) ? "PENUH" : "KOSONG";
  String statusParkir5 = (!IR5) ? "PENUH" : "KOSONG";
  String statusParkir6 = (!IR6) ? "PENUH" : "KOSONG";

  lcd.setCursor(0,0);  lcd.print("     SLOT PARKIR    ");
  lcd.setCursor(0,1);  lcd.print("P1:"); lcd.print(statusParkir1);  lcd.print("  ");
  lcd.setCursor(10,1); lcd.print("P2:"); lcd.print(statusParkir2);  lcd.print("  ");
  lcd.setCursor(0,2);  lcd.print("P3:"); lcd.print(statusParkir3);  lcd.print("  ");
  lcd.setCursor(10,2); lcd.print("P4:"); lcd.print(statusParkir4);  lcd.print("  ");
  lcd.setCursor(0,3);  lcd.print("P5:"); lcd.print(statusParkir5);  lcd.print("  ");
  lcd.setCursor(10,3); lcd.print("P6:"); lcd.print(statusParkir6);  lcd.print("  ");

  if(millis() - currenTime > 1500){
    if(!pintuMasuk){
      statusServoMasuk =! statusServoMasuk;
      if(statusServoMasuk){
        servoPintuMasuk.write(90);
      } else {
        servoPintuMasuk.write(0);
      } currenTime = millis();
    } else if (!pintuKeluar){
      statusServoKeluar =! statusServoKeluar;
      if(statusServoKeluar){
        servoPintuKeluar.write(90);
      } else {
        servoPintuKeluar.write(0);
      } currenTime = millis();
    }
  }
  
}
