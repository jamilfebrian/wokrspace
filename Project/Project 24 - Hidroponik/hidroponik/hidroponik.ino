#define BLYNK_TEMPLATE_ID "TMPL6x7TTtErr"
#define BLYNK_TEMPLATE_NAME "Smart Hidroponik"
#define BLYNK_AUTH_TOKEN "ZSBheV-ynltvShXNzyqBRC4PhvqxiRkf"

#include <OneWire.h>
#include <DallasTemperature.h>
#include <LiquidCrystal_I2C.h>
#include <NewPing.h>
#include <EEPROM.h> 
#include <WiFi.h>
#include <HTTPClient.h>
#include "BlynkSimpleEsp32.h"

#define ssid "192.168.50.10"
#define password "12345678"
#define token BLYNK_AUTH_TOKEN
#define Web_App_URL "https://script.google.com/macros/s/AKfycbxLy5fZNoZdtKWwruCF6OKgYjV6nqg5xTc028pO1vquhkDTowiet06cZ2s0S9xwdBjllA/exec?"

#define DS18B20       4
#define TDSSensorPin 35
#define pHSensorPin  36
#define BuzzerPin    23
#define relayPin1    14
#define relayPin2    27
#define TRIGPIN      18
#define ECHOPIN      19

BlynkTimer timer;
OneWire oneWire(DS18B20);
DallasTemperature sensors(&oneWire);
LiquidCrystal_I2C lcd(0x27, 16, 2);
NewPing sonar(TRIGPIN, ECHOPIN, 200); 

unsigned long timeout = 0;
String Status_Read_Sensor = "SUCCES";

float voltage_ph7 = 2.95;
float voltage_ph4 = 3.30;

int PANJANG = 50;
int LEBAR = 40;

static float temperatureC;
static float temperatureF;

static float tdsValue;
static float pHValue;

static int volume ;

byte derajat[8] = {
    0b00111,
    0b00101,
    0b00111,
    0b00000,
    0b00000,
    0b00000,
    0b00000,
    0b00000
  };

void buzzer(int index = 3, int timer = 80){
    for(int i=0; i<index; i++){
      digitalWrite(BuzzerPin, HIGH);
      delay(timer);
      digitalWrite(BuzzerPin, LOW);
      delay(timer);
  }
}

void connectWifi(){
  buzzer(1);
  WiFi.mode(WIFI_STA);
  lcd.setCursor(0,0); lcd.print(" Menghubungkan  ");
  lcd.setCursor(0,1); lcd.print(" Ke : "+ String(ssid) +"     ");
  WiFi.begin(ssid, password);
  Blynk.begin(token, ssid, password); 
  while (WiFi.status() != WL_CONNECTED) {
    delay(500); 
  } buzzer(3);
  lcd.setCursor(0,0); lcd.print("    Jaringan    ");
  lcd.setCursor(0,1); lcd.print(" Wifi Terhubung ");
}

void sendDataToSpreadsheet(){
    String Send_Data_URL = Web_App_URL "sts=write";
    
    Send_Data_URL += "&srs=" + Status_Read_Sensor;
    Send_Data_URL += "&V=" + String(volume);
    Send_Data_URL += "&TDS=" + String(tdsValue);
    Send_Data_URL += "&PH=" + String(pHValue);
    Send_Data_URL += "&T=" + String(temperatureC);

      HTTPClient http;
      http.begin(Send_Data_URL.c_str());
      http.setFollowRedirects(HTTPC_STRICT_FOLLOW_REDIRECTS);

      int httpCode = http.GET(); 
      
      String payload;
      if (httpCode > 0) {
        payload = http.getString();
      }
      http.end();
}

void setup() {
  Serial.begin(115200);
  pinMode(pHSensorPin, INPUT);
  pinMode(TDSSensorPin, INPUT);
  pinMode(relayPin1, OUTPUT);
  pinMode(relayPin2, OUTPUT);

  digitalWrite(relayPin1,LOW);
  digitalWrite(relayPin2,HIGH);

  lcd.init(); lcd.backlight(); lcd.clear();
  sensors.begin();
  
  connectWifi();
  lcd.createChar(0, derajat); 
  timer.setInterval(300000L, sendDataToSpreadsheet);
}

void loop() {
  
  timer.run();  Blynk.run(); 
  sensors.requestTemperatures(); 
   temperatureC = sensors.getTempCByIndex(0);
   temperatureF = sensors.getTempFByIndex(0);
   
//========TDS=======================================================
  int analogValue = analogRead(TDSSensorPin);  
  float voltage = analogValue * (3.3 / 4096); 
  float compensationCoefficient = 1.0 + 0.02 * (temperatureC - 29.0);
  float compensationVoltage = voltage / compensationCoefficient; 
  tdsValue = (133.42 * compensationVoltage * compensationVoltage * compensationVoltage 
                   - 255.86 * compensationVoltage * compensationVoltage 
                   + 857.39 * compensationVoltage) * 0.5;
               Serial.print(analogValue);  Serial.print("|");
               Serial.println(tdsValue);
               
//========Ph=======================================================
   int ph = 0;
   for(int i=0; i<10; i++){
    ph = ph + analogRead(pHSensorPin);
    delay(5);
   } float analogValuePh = float(ph)/10;
   float voltagePh = analogValuePh * (3.3/4095);
   pHValue = 7 + (voltagePh - voltage_ph7) * ((4 - 7) / (voltage_ph4 - voltage_ph7));

//========HC04=======================================================
   static int TINGGI = 30 - sonar.ping_cm();
   volume = (PANJANG * LEBAR * TINGGI)/1000;

  if (timeout<millis()){
    Blynk.virtualWrite(V0, temperatureC); 
    Blynk.virtualWrite(V1, (int)tdsValue); 
    Blynk.virtualWrite(V2, volume); 
    Blynk.virtualWrite(V3, pHValue); 

    lcd.setCursor(0,0); lcd.print("T:"); lcd.print(temperatureC); lcd.write(byte(0));  lcd.print("C"); 
                        lcd.print(" pH:"); lcd.print(pHValue); lcd.print("  ");
    lcd.setCursor(0,1); lcd.print("TDS:"); lcd.print((int)tdsValue); 
                        lcd.print(" V:"); lcd.print(volume); lcd.print("L  ");

    timeout = millis() + 1000;
  }
}

BLYNK_WRITE(V4){
  int value = param.asInt();  
  if (value == 1) {
   digitalWrite(relayPin2,LOW);
  } else {
   digitalWrite(relayPin2,HIGH);
  }
}
