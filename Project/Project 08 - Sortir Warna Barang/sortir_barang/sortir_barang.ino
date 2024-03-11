#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include "Adafruit_TCS34725.h"
#include <Servo.h>

Adafruit_TCS34725 tcs = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_600MS, TCS34725_GAIN_1X);
Servo servodorong, servogate1, servogate2, servogate3;
LiquidCrystal_I2C lcd(0x27,16,2);

#define servoDorong   6
#define servoGate1    9
#define servoGate2   10
#define servoGate3   11
#define kotak1        2
#define kotak2        3
#define kotak3        4
#define kotak4        5
#define kotakseleksi  7
#define conveyor     13

boolean logic=false, logicR=false, logicG=false, logicB=false, logicC=false;   //logika untuk servo
unsigned long waktuSensorSekarang = 0, pTime=0, waktuLCD=0;
unsigned long waktuSensor = 500, timer=3000, intervalLCD=1500;
const int jumlahBaca = 10;
int bacaR[jumlahBaca], bacaG[jumlahBaca], bacaB[jumlahBaca], bacaC[jumlahBaca];
int indexBacaR=0, indexBacaG=0, indexBacaB=0, indexBacaC=0;
int totalR=0, totalG=0, totalB=0, totalC=0;     
int redBox=0, greenBox=0,blueBox=0, yellowBox=0;                //simpan jumlah data barang
int kotakSeleksi,kotakMerah, kotakHijau,kotakBiru,kotakKuning; //ambil data sensor
int sudutServo = 65, sudutAwal=7, lcdtampilan=0;                     // sudut servo barang, dan mode tampilan LCD
String dataKirim = "";

struct dataWarna{
  int red;
  int green;
  int blue;
  int cyan;
};

dataWarna deteksiWarna(){
  static dataWarna average;
  uint16_t r, g, b, c;
  while(true){
    c = tcs.read16(TCS34725_CDATAL);
    r = tcs.read16(TCS34725_RDATAL);
    g = tcs.read16(TCS34725_GDATAL);
    b = tcs.read16(TCS34725_BDATAL);    
    totalR = totalR - bacaR[indexBacaR];
    totalG = totalG - bacaG[indexBacaG];
    totalB = totalB - bacaB[indexBacaB];
    totalC = totalC - bacaC[indexBacaC];
    bacaR[indexBacaR] = r;
    bacaG[indexBacaG] = g;
    bacaB[indexBacaB] = b;
    bacaC[indexBacaC] = c;
    totalR = totalR + bacaR[indexBacaR];
    totalG = totalG + bacaG[indexBacaG];
    totalB = totalB + bacaB[indexBacaB];
    totalC = totalC + bacaC[indexBacaC];
    indexBacaR = indexBacaR + 1;
    indexBacaG = indexBacaG + 1;
    indexBacaB = indexBacaB + 1;
    indexBacaC = indexBacaC + 1;  
    if (indexBacaR >= jumlahBaca) {
      average.red = totalR / (jumlahBaca - 1);
      indexBacaR = 0;
    }
    if (indexBacaG >= jumlahBaca) {
      average.green = totalG / (jumlahBaca - 1);
      indexBacaG = 0;
    }
    if (indexBacaB >= jumlahBaca) {
      average.blue = totalB / (jumlahBaca - 1);
      indexBacaB = 0;  
    }
    if (indexBacaC >= jumlahBaca) {
      average.cyan = totalC / (jumlahBaca - 1);
      indexBacaC = 0;  break;
    }
  }
      return average;
}

void servo(){
  if((millis()-pTime)>timer && kotakSeleksi==1){
      for (int pos = 0; pos <= 55; pos++){
        servodorong.write(pos);
        delay(5); 
      } for (int pos = 55 ; pos >= 0; pos--){
        servodorong.write(pos);
        delay(5); 
     } pTime=millis();
   }
}

void displayLCD(){
  if(lcdtampilan == 1){
    lcd.setCursor(0,0);
    lcd.print("Kotak Merah : ");
    lcd.print(String(redBox)+"  ");
    lcd.setCursor(0,1);
    lcd.print("Kotak Hijau : ");
    lcd.print(String(greenBox)+"  ");
  } else if(lcdtampilan == 2){
    lcd.setCursor(0,0);
    lcd.print("Kotak Biru  : ");
    lcd.print(String(blueBox)+"  "); 
    lcd.setCursor(0,1);
    lcd.print("Kotak Kning : ");
    lcd.print(String(yellowBox)+"  "); 
  } else {
    lcd.clear();
  }
}
void setup() {
  Serial.begin(9600);
  pinMode(kotak1,INPUT);
  pinMode(kotak2,INPUT);
  pinMode(kotak3,INPUT);
  pinMode(kotak4,INPUT);
  pinMode(kotakseleksi,INPUT);
  pinMode(conveyor,OUTPUT);
  digitalWrite(conveyor, LOW);
  servodorong.attach(servoDorong); servodorong.write(0);
  servogate1.attach(servoGate1); servogate1.write(sudutServo);
  servogate2.attach(servoGate2); servogate2.write(sudutServo);
  servogate3.attach(servoGate3); servogate3.write(sudutServo);
  lcd.begin();
  lcd.clear();
  lcd.backlight();

  lcd.setCursor(0,0);
  lcd.print(" Annes Vardila");
  lcd.setCursor(4,1);
  lcd.print("18130003");
  delay(2000);  
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Penyortir Warna");
  lcd.setCursor(4,1);
  lcd.print("Barang");
  delay(2000); 
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("    Memulai     ");
  lcd.setCursor(0,1);
  lcd.print("    TCS34725    ");
  
  if (tcs.begin()){
//    Serial.println("Found sensor");
    lcd.setCursor(0,0);
    lcd.print("    TCS34725    ");
    lcd.setCursor(0,1);
    lcd.print("     Online     ");
    delay(1500);
  } else {
//    Serial.println("No TCS34725 found ... check your connections");
    lcd.setCursor(0,0);
    lcd.print("    TCS34725    ");
    lcd.setCursor(0,1);
    lcd.print("   Not  Found   ");
    while (1);
  }
}

void loop() {
  dataWarna dataR = deteksiWarna();
  kotakMerah = digitalRead(kotak1);
  kotakHijau = digitalRead(kotak2);
  kotakKuning = digitalRead(kotak3);
  kotakBiru = digitalRead(kotak4);
  kotakSeleksi = digitalRead(kotakseleksi);
  
  if(!logic){
    if(dataR.red>275 && dataR.red<340 && dataR.green>60 && dataR.green<120 && dataR.blue>65 && dataR.blue<100 && dataR.cyan>370 && dataR.cyan<470 ){  
//        Serial.println("Merah");
        digitalWrite(conveyor, HIGH);
        servo();  logicR=true; logic=true;
        servogate1.write(sudutAwal);
     } else if (dataR.red>90 && dataR.red<120 && dataR.green>80 && dataR.green<120 && dataR.blue>45 && dataR.blue<70 && dataR.cyan>200 && dataR.cyan<270 ){
//        Serial.println("Hijau");
        digitalWrite(conveyor, HIGH);
        servo();  logicG=true;  logic=true;
        servogate2.write(sudutAwal);
     } else if (dataR.red>380 && dataR.red<480 && dataR.green>200 && dataR.green<265 && dataR.blue>100 && dataR.blue<265 && dataR.cyan>650 && dataR.cyan<830 ){
//        Serial.println("Kuning");
        digitalWrite(conveyor, HIGH);
        servo();  logicC=true; logic=true;
        servogate3.write(sudutAwal);
     } else if (dataR.red>60 && dataR.red<90 && dataR.green>40 && dataR.green<75 && dataR.blue>40 && dataR.blue<70 && dataR.cyan>130 && dataR.cyan<210 ){
//        Serial.println("Biru");
        digitalWrite(conveyor, HIGH);
        servo();  logicB=true; logic=true;
        servogate3.write(sudutServo);
     } else {
//       Serial.println("Tidak Terdeteksi");
     }
  }
   if(logic){
   if(kotakMerah==1 && logicR==true){
      servogate1.write(sudutServo);
      redBox++;  delay(500); 
      logicR=false; logic=false;
      digitalWrite(conveyor, LOW);
   } else if(kotakHijau==1 && logicG==true){
      servogate2.write(sudutServo);
      greenBox++; delay(500); 
      logicG=false; logic=false;
      digitalWrite(conveyor, LOW);
   } else if(kotakKuning==1 && logicC==true){
      servogate3.write(sudutServo);
      yellowBox++;  delay(500); 
      logicB=false; logic=false;
      digitalWrite(conveyor, LOW);
   } else if(kotakBiru==1 && logicB==true){
      servogate3.write(sudutServo);
      blueBox++;  delay(500); 
      logicC=false; logic=false;
      digitalWrite(conveyor, LOW);
   } 
  }

  if((millis()-waktuLCD)>intervalLCD){
    lcdtampilan++;
    if(lcdtampilan>=3){
      lcdtampilan=1;
    } waktuLCD=millis();
  }
  displayLCD();

  dataKirim = String(redBox)+" "+String(greenBox)+" "+String(blueBox)+" "+String(yellowBox);
  
  String minta = "";
  while(Serial.available()>0){
    minta += char(Serial.read());
  } minta.trim();
  if (minta == "YA"){
    Serial.println(dataKirim);
  }   minta = "";
}
