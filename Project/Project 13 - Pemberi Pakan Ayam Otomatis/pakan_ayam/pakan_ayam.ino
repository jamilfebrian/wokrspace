#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>

#include <DHT.h>
#include <WiFi.h>
#include <Wire.h> 
#include <Servo.h>
#include <LiquidCrystal_I2C.h>
#include "RTClib.h"
#include <HX711_ADC.h>
#include <EEPROM.h>

#define BLYNK_TEMPLATE_ID "TMPL6NhI0IKZj"
#define BLYNK_TEMPLATE_NAME "Pakan ayam"
#define BLYNK_AUTH_TOKEN "CeoOFVwSmkcTBGZkbV4V335pQd2gQEcg"
#define ssid      "realme C31"
#define password   "987654321" 
#define auth BLYNK_AUTH_TOKEN

#define pWindowL 27 
#define pWindowR 26
#define relay_pump 25
#define relay_lamp 33
#define servo_wadah_pin 14
#define servo_pakan_pin 12

const int IR[5] = {32,13,34,36,39}; // vp.vn
const int water_level_pin = 35;
const int HX711_dout = 19;
const int HX711_sck = 23;

void dataBlynk();
DHT dhtSensors[5] = {
  DHT(4 , DHT11),
  DHT(16, DHT11),
  DHT(17, DHT11),
  DHT(5 , DHT11),
  DHT(18, DHT11)};

Servo servo_pakan;
Servo servo_wadah;

HX711_ADC LoadCell(HX711_dout, HX711_sck);
const int calVal_eepromAdress = 0;
long t;

BlynkTimer timer;
RTC_DS3231 rtc;
static DateTime targetTime;
LiquidCrystal_I2C lcd(0x27,16,2); 
const byte Simbol_derajat = B11011111;
static bool IRValue[5];//menyimpan nilai IR
static float suhu[5]; //menyimpan nilai suhu
static int waterLevelPercentage;    //menyimpan nilai level air
static int loadCellValue;

unsigned long cTime=0;
const int jeda_servo = 1000;
unsigned short int gerakanMotor = 0;
bool kosong = true, jeda=false;
float calibrationValue = 696.0;

void connectWifi(){
  pinMode(2,OUTPUT);
  digitalWrite(2,LOW);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000); digitalWrite(2, LOW);
    Serial.println("Connecting to WiFi...");
  }  Serial.println("Wifi Terhubung");
   Blynk.begin(auth, ssid, password, "blynk.cloud", 80);
  digitalWrite(2,HIGH);
}


//========pembacaan load cell=========//
void loadCell(){
  static boolean newDataReady = 0;
  const int serialPrintInterval = 0; 
  if (LoadCell.update()) newDataReady = true;
  if (newDataReady) {
    if (millis() > t + serialPrintInterval) {
      loadCellValue = LoadCell.getData();
      if(loadCellValue<0){
        loadCellValue=0;
      }
      newDataReady = 0;
      t = millis();
    }
  }
  if(loadCellValue <= 20){
    Blynk.logEvent("pakan_habis","Pakan Telah Habis,Segera Isi Kembali!"); 
  }
}

//=========pembacaan Infared===========//
void infraRed(){
  for(int i=0; i<5;i++){
    pinMode(IR[i], INPUT);
    IRValue[i] = digitalRead(IR[i]);
  }
}

//===========pembacaan DHT11============//
float dht(DHT &dhtSensor) {
  float temperature = dhtSensor.readTemperature();
  return temperature;
}

void bacaSuhu(){
  for (int i=0; i<5; i++) {
    suhu[i] = dht(dhtSensors[i]);
  } delay(10);
}

//===========pembacaan water level============//
void WaterLevel(){
  int sensorValue = analogRead(water_level_pin);
  waterLevelPercentage = map(sensorValue, 0, 4095, 0, 100);
  
  if(waterLevelPercentage>=100){
    digitalWrite(relay_pump,HIGH);
  } else if(waterLevelPercentage<=50){
    digitalWrite(relay_pump,LOW);
    Blynk.logEvent("air_habis","Air Habis, Mengisi Kembali!"); 
  }
}

//============kontrol arah motor================//
void control_motor(String index){
  if(index=="mundur"){ //mundur
    digitalWrite(pWindowL,HIGH);
    digitalWrite(pWindowR,LOW);
  } else if(index=="maju"){ //maju
    digitalWrite(pWindowL,LOW);
    digitalWrite(pWindowR,HIGH);
  } else { 
    digitalWrite(pWindowL,HIGH);
    digitalWrite(pWindowR,HIGH);
  }
}

//======atur pergerakan Servo========//
void servo(int index){
  if(index==0){
    servo_wadah.write(60);
  } else if(index==1){
    servo_pakan.write(180);
  } else if(index==10){
    servo_wadah.write(150);
  } else if(index==11){
    servo_pakan.write(85);
  } else {
    servo_wadah.write(150);
    servo_pakan.write(85);
  }
}

void tampilNamaPadaLCD(){
  if(millis()-cTime>12000 && millis()-cTime<14000){
    lcd.setCursor(0,0);
    lcd.print("  Tugas  Akhir  ");
    lcd.setCursor(0,1);
    lcd.print("                ");
  } else if(millis()-cTime>14000 && millis()-cTime<16000){
    lcd.setCursor(0,0);
    lcd.print("Mia Nurkhalifah ");
    lcd.setCursor(0,1);
    lcd.print("Riko Simanjuntak");
  } else if(millis()-cTime>16000 && millis()-cTime<18000){
    lcd.setCursor(0,0);
    lcd.print("Pemberian Pakan ");
    lcd.setCursor(0,1);
    lcd.print(" Ayam Otomatis  ");
  } else if(millis()-cTime>18000 && millis()-cTime<19000){
    lcd.clear();
  } else if(millis()-cTime>19000 && millis()-cTime<21000){
    lcd.setCursor(0,0);
    lcd.print("Suhu 1 : ");
    lcd.print(suhu[0]); lcd.print("               ");
    lcd.setCursor(0,1);
    lcd.print("Suhu 2 : ");
    lcd.print(suhu[1]); lcd.print("               ");
  } else if(millis()-cTime>21000 && millis()-cTime<23000){
    lcd.setCursor(0,0);
    lcd.print("Suhu 3 : ");
    lcd.print(suhu[2]); lcd.print("               ");
    lcd.setCursor(0,1);
    lcd.print("Suhu 4 : ");
    lcd.print(suhu[3]); lcd.print("               ");
  } else if(millis()-cTime>23000 && millis()-cTime<25000){
    lcd.setCursor(0,0);
    lcd.print("Suhu 5 : ");
    lcd.print(suhu[4]); lcd.print("               ");
    lcd.setCursor(0,1);
    lcd.print("W Level: ");
    lcd.print(waterLevelPercentage);
    lcd.print(" %");    lcd.print("               ");
  } else if(millis()-cTime>25000){
    cTime=millis()-19000;
  } 
}

void setup(){
  Serial.begin(9600);
  pinMode(water_level_pin, INPUT);
  pinMode(pWindowL, OUTPUT);
  pinMode(pWindowR, OUTPUT);
  
    pinMode(relay_pump, OUTPUT);
    pinMode(relay_lamp, OUTPUT); 
    digitalWrite(relay_pump,HIGH);
    digitalWrite(relay_lamp,LOW);

  pinMode(HX711_dout, INPUT);
  LoadCell.begin();
  EEPROM.get(calVal_eepromAdress, calibrationValue);
  LoadCell.start(2000, true);
  if (LoadCell.getTareTimeoutFlag()) {
    Serial.println("Timeout, cek kabel MCU>HX711 pastikan sudah tepat");
    while (1);
  } else {
    LoadCell.setCalFactor(calibrationValue);
    Serial.println("Startup selesai");
  }

      servo_wadah.attach(servo_wadah_pin);
      servo_pakan.attach(servo_pakan_pin);
      servo_wadah.write(150);
      servo_pakan.write(85);
  
  for (int i=0; i<5; i++) {
    dhtSensors[i].begin(); }
  Wire.begin();
  rtc.begin();
  rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));

      lcd.begin();
      lcd.clear();
      lcd.backlight();
      connectWifi();

  timer.setInterval(100L, WaterLevel);
  timer.setInterval(100L, bacaSuhu);
  timer.setInterval(100L, infraRed);
  timer.setInterval(100L, tampilNamaPadaLCD);
  timer.setInterval(100L, dataBlynk);
  timer.setInterval(100L, loadCell);

  control_motor("mundur");
}

void loop(){
  if(!jeda){
    tampilNamaPadaLCD();
  }
  
  Blynk.run();
  WaterLevel();
  bacaSuhu();
  infraRed();
  loadCell();
  DateTime now = rtc.now();

  if (!IRValue[0] && kosong && !jeda){
    control_motor("stop");
    servo(0);
    delay(jeda_servo);
    servo(10);
    if(gerakanMotor>0){
      control_motor("maju");
      delay(500);
    } kosong=false;
  }
  
  if(gerakanMotor==0 && !IRValue[0] && !kosong){
      control_motor("stop");
      servo(1);
      delay(jeda_servo);
      servo(11);
      gerakanMotor++;
      kosong=true;
    }

  if(gerakanMotor==1 && !IRValue[1] && !kosong){
      control_motor("stop");
      servo(1);
      delay(jeda_servo);
      servo(11);
      control_motor("mundur");
      gerakanMotor++;
      kosong=true;
  }

  if(gerakanMotor==2 && !IRValue[2] && !kosong){
      control_motor("stop");
      servo(1);
      delay(jeda_servo);
      servo(11);
      control_motor("mundur");
      gerakanMotor++;
      kosong=true;
  }

  if(gerakanMotor==3 && !IRValue[3] && !kosong){
      control_motor("stop");
      servo(1);
      delay(jeda_servo);
      servo(11);
      control_motor("mundur");
      gerakanMotor++;
      kosong=true;
  }

  if(gerakanMotor==4 && !IRValue[4] && !kosong){
      control_motor("stop");
      servo(1);
      delay(jeda_servo);
      servo(11);
      control_motor("mundur");
      gerakanMotor=0;
      kosong=true;
      targetTime = now + TimeSpan(0, 0, 10, 0);
  }

  if((targetTime-now).hours()>0 || (targetTime-now).minutes()>0 || (targetTime-now).seconds()>0){
      jeda = true;
      lcd.setCursor(0,0);
      lcd.print("Jeda Waktu :    ");
      lcd.setCursor(0,1);
      lcd.print((targetTime-now).hours());
      lcd.print(" : ");
      lcd.print((targetTime-now).minutes());
      lcd.print(" : ");
      lcd.print((targetTime-now).seconds());
      lcd.print("              ");
      control_motor("stop");
    } else {
      if(!IRValue[4] && jeda){
        control_motor("mundur");
      } jeda = false;
    }
    dataBlynk();
}

void dataBlynk(){
  Blynk.virtualWrite(V2, suhu[0]); 
  Blynk.virtualWrite(V3, suhu[1]); 
  Blynk.virtualWrite(V4, suhu[2]); 
  Blynk.virtualWrite(V5, suhu[3]); 
  Blynk.virtualWrite(V6, suhu[4]); 
  Blynk.virtualWrite(V7, waterLevelPercentage); 
  Blynk.virtualWrite(V0, loadCellValue); 
}

BLYNK_WRITE(V1){
  int value = param.asInt();  
  if (value == 1) {
   digitalWrite(relay_lamp,HIGH);
  } else {
   digitalWrite(relay_lamp,LOW);
  }
}
BLYNK_WRITE(V8){
  int value = param.asInt();  
  if (value == 1) {
   LoadCell.tareNoDelay();
  } else {
  }
}
