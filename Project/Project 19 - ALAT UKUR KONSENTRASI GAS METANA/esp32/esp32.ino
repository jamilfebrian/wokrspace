#define BLYNK_TEMPLATE_ID "TMPL6R6Zgjx6p"
#define BLYNK_TEMPLATE_NAME "Biodiagaster metana meter control"
#define BLYNK_AUTH_TOKEN "ufq-ukh8suzjr-a9XgfK-urLNl34Lc2I"

#include <WiFi.h>
#include <BlynkSimpleEsp32.h>
#include <LiquidCrystal_I2C.h>
#include "Adafruit_VL53L0X.h"

#define ssid "Qoqo"
#define password "qoqookee"
//#define ssid "Tinkpad"
//#define password "12345678"
#define token BLYNK_AUTH_TOKEN

WidgetLCD lcdBlynk1(V9);
LiquidCrystal_I2C lcd(0x27, 16, 2);
Adafruit_VL53L0X lox = Adafruit_VL53L0X();

#define MQ4_A0  34
#define MQ4_D0  35
#define relay1  18  //relay untuk membuka katup selenoid
#define relay2  19  //relay untuk menggerakkan motor DC
#define BUZZER  13

const float CH4_RL = 10.00;  // Load Resistance in kΩ
const float CH4_R0 = 100.00;  // Resistansi ketika udara bersih
const float CH4_Vin = 3.30;  // tegangan input dari esp32
const int CH4_Bit = 4095;    // bit data esp32
static float CHVal;

const float a = -2.74; //konstanta kurva kalibrasi dari datasheet
const float b = -1.5;  //konstanta kurva kalibrasi dari datasheet
bool read_sensor = false;

int CH4_value;
int CH4_percent;
float CH4_Vout;
float CH4_RS;
float CH4_ratio;
float CH4_PPM;
float CH4_PPM_Percent;

long mappedValue;
int Sludge_range;

void buzzer(int index = 3, int timer = 80){
    for(int i=0; i<index; i++){
      digitalWrite(BUZZER, HIGH);
      delay(timer);
      digitalWrite(BUZZER, LOW);
      delay(timer);
  }
}

void setup() {
  Serial.begin(115200);
  lcd.init(); lcd.backlight(); lcd.clear();
  WiFi.begin(ssid, password);
  Blynk.begin(token, ssid, password, "blynk.cloud", 80);
  
  lcdBlynk1.clear();
  
  pinMode(MQ4_A0,INPUT);
  pinMode(MQ4_D0,INPUT);
  pinMode(relay1, OUTPUT);
  pinMode(relay2, OUTPUT);
  pinMode(BUZZER, OUTPUT);

  digitalWrite(relay1, HIGH);
  digitalWrite(relay2, HIGH);

  if(!digitalRead(relay1)){
    Blynk.virtualWrite(V0,HIGH);
  }else{
    Blynk.virtualWrite(V0,LOW);
  }

  if (!lox.begin()) {
    Serial.println(F("Failed to boot VL53L0X"));
    while(1);
  } lox.startRangeContinuous();

  CHVal = 0; //analogRead(MQ4_A0);
}

void loop() {
  Blynk.run();
  if(read_sensor){
     CH4_value = analogRead(MQ4_A0) - CHVal;                      // nilai ADC sensor gas
     CH4_percent = CH4_value * 100 / (CH4_Bit-CHVal);             // persentase nilai ADC sensor gas
     CH4_Vout = CH4_Vin * float(CH4_value) / CH4_Bit;   // nilai tegangan output pada sensor gas
     CH4_RS = ((CH4_Vin-CH4_Vout) * CH4_RL) / CH4_Vout; // nilai resistansi variabel gas

     CH4_ratio = (CH4_RS / CH4_R0);
     CH4_PPM = 1000*pow(CH4_ratio, -2.74);  
     CH4_PPM_Percent = CH4_PPM / 10000;
  
     mappedValue = map(CH4_ratio, 1, 2, 1000, 200);
    
    Sludge_range = lox.readRange()/10;
    Sludge_range = (Sludge_range >= 100) ? 100 : Sludge_range;
  }
  
  Serial.print(CH4_ratio); Serial.print(" ");
  Serial.println(CH4_PPM_Percent);

  Blynk.virtualWrite(V1, mappedValue);  
  Blynk.virtualWrite(V2, Sludge_range); 
 
  lcdBlynk1.print(0,0,"CH4%:"+String(CH4_percent)+" PPM:"+String(CH4_PPM)+" ");

  lcd.setCursor(0, 0);
  lcd.print("R:"); lcd.print(CH4_ratio); 
  lcd.print(" PPM:");lcd.print(CH4_PPM); lcd.print("  ");
  lcd.setCursor(0, 1);
  lcd.print("Slg:"); lcd.print(Sludge_range); lcd.print("  ");
  
  if(Sludge_range >= 100){
    lcd.setCursor(9, 1);
    lcd.print("(Empty)   ");
    lcdBlynk1.print(0,1,"Sludge : (Empty)  ");
  } else if(Sludge_range < 30){
    lcd.setCursor(9, 1);
    lcd.print("(Full)   ");
    lcdBlynk1.print(0,1,"Sludge : (Full)  ");
  } else {
    lcd.setCursor(9, 1);
    lcd.print("(Fill)   ");
    lcdBlynk1.print(0,1,"Sludge : (Fill) ");
  }

  if(CH4_percent >= 55){
    digitalWrite(relay2, LOW);
    Blynk.logEvent("ch4_65","Gas Metana CH4 mencapai 65% "); 
    buzzer(8,80);
  } else {
    digitalWrite(relay2, HIGH);
  }
}

BLYNK_WRITE(V0){
  int pinValue = param.asInt();
  if(pinValue){
    digitalWrite(relay1, LOW);
  } else {
    digitalWrite(relay1, HIGH);
  }
}

BLYNK_WRITE(V10){
  int pinValue = param.asInt();
  if(pinValue){
    lcd.init();
    lox.begin();
  } else {
    // nothing code
  }
}

BLYNK_WRITE(V11){
  int pinValue = param.asInt();
  if(pinValue){
    read_sensor = true;
  } else {
    // nothing code
  }
}
