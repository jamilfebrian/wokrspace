#define BLYNK_TEMPLATE_ID "TMPL61IxapWdA"
#define BLYNK_TEMPLATE_NAME "Project Testing"
#define BLYNK_AUTH_TOKEN "ICfMQqpE867-F7YvRUOehw5CPgQO8pGz"

#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
#include <Adafruit_MLX90614.h>
#include <NewPing.h>
#include "Arduino.h"
#include "DFRobotDFPlayerMini.h"
#include "WiFi.h"
#include <BlynkSimpleEsp32.h>

#include <HX711_ADC.h>
#include <EEPROM.h>

#define HX711_dout 18
#define HX711_sck  23

HX711_ADC LoadCell(HX711_dout, HX711_sck);
const int calVal_eepromAdress = 0;
unsigned long weight = 0;

#define ssid      "192.168.50.10"   
#define password  "12345678" 
#define auth  BLYNK_AUTH_TOKEN 

#define PressureSensorPin 34
#define motorRelay 25
#define selenoid 27

#define TRIGGER_PIN  12  
#define ECHO_PIN     14

Adafruit_MLX90614 mlx = Adafruit_MLX90614();
LiquidCrystal_I2C lcd(0x27,20,4);  
NewPing sonar(TRIGGER_PIN, ECHO_PIN, 200); 
DFRobotDFPlayerMini myDFPlayer;

unsigned long cTime = 0;
unsigned long cTime1 = 0;
unsigned long t = 0;
int dataadc;
float mmhg;

#define DFVolume 20

void loadCell(){
  Serial.println();
  Serial.println("Starting...");

  LoadCell.begin();
  LoadCell.start(2000, true);
  if (LoadCell.getTareTimeoutFlag() || LoadCell.getSignalTimeoutFlag()) {
    Serial.println("Timeout, check MCU>HX711 wiring and pin designations");
    while (1);
  } else {
    LoadCell.setCalFactor(23.41); 
    Serial.println("Startup is complete");
  }
  while (!LoadCell.update());
//  calibrate(); 
  Serial.print("Calibration value: ");
  Serial.println(LoadCell.getCalFactor());
  Serial.print("HX711 measured conversion time ms: ");
  Serial.println(LoadCell.getConversionTime());
  Serial.print("HX711 measured sampling rate HZ: ");
  Serial.println(LoadCell.getSPS());
  Serial.print("HX711 measured settling time ms: ");
  Serial.println(LoadCell.getSettlingTime());
  Serial.println("Note that the settling time may increase significantly if you use delay() in your sketch!");
  if (LoadCell.getSPS() < 7) {
    Serial.println("!!Sampling rate is lower than specification, check MCU>HX711 wiring and pin designations");
  } else if (LoadCell.getSPS() > 100) {
    Serial.println("!!Sampling rate is higher than specification, check MCU>HX711 wiring and pin designations");
  }
}

float loadCellData(){
  static boolean newDataReady = 0;
  const int serialPrintInterval = 0; 
  static float berat = 0;

  if (LoadCell.update()) newDataReady = true;
  if (newDataReady) {
    if (millis() > t + serialPrintInterval) {
      berat = LoadCell.getData();
      newDataReady = 0;
      t = millis();
    }
  }

  if (Serial.available() > 0) {
    char inByte = Serial.read();
    if (inByte == 't') LoadCell.tareNoDelay(); //tare
    else if (inByte == 'r') calibrate(); //calibrate
    else if (inByte == 'c') changeSavedCalFactor(); 
  }

  if (LoadCell.getTareStatus() == true) {
    Serial.println("Tare complete");
  }
  return  berat;
}

void connectWifi(){
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Blynk.begin(auth, ssid, password, "blynk.cloud", 80);
  while (WiFi.status() != WL_CONNECTED) {
        delay(500);
    }
}

void setup() {
  Wire.begin();
  Serial.begin(115200);

  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.clear();
  lcd.setCursor(4, 0);
  lcd.print("IGO FIRNANDES");
  lcd.setCursor(6, 1);
  lcd.print("19065008");
  lcd.setCursor(0, 2);
  lcd.print("P.TEKNIK ELEKTRONIKA");
  lcd.setCursor(3, 3);
  lcd.print("FAKULTAS TEKNIK");
  delay(2000);

  loadCell();
  Serial.println("HX711 siap.");

  #if (defined ESP32)
    Serial2.begin(9600, SERIAL_8N1, /*rx =*/16, /*tx =*/17);
  #else
    Serial2.begin(9600);
  #endif

  if (!myDFPlayer.begin(Serial2, /*isACK = */true, /*doReset = */true)) { 
    Serial.println(F("Unable to begin:"));
    Serial.println(F("1.Please recheck the connection!"));
    Serial.println(F("2.Please insert the SD card!"));
    while(true);
  } Serial.println(F("DFPlayer Mini online."));
  myDFPlayer.setTimeOut(500);
  myDFPlayer.volume(DFVolume); 
  myDFPlayer.EQ(DFPLAYER_EQ_NORMAL);
  myDFPlayer.outputDevice(DFPLAYER_DEVICE_SD);

  lcd.init(); lcd.backlight(); lcd.clear();
  lcd.setCursor(0,0); lcd.print("Menghubungkan   ");
  lcd.setCursor(0,1); lcd.print("Ke : " + String(ssid));
  
  pinMode(motorRelay, OUTPUT);
  pinMode(selenoid, OUTPUT);

  digitalWrite(selenoid, HIGH);
  digitalWrite(motorRelay, HIGH);

  mlx.begin(0x5A);
  connectWifi();
  
  lcd.setCursor(0,0); lcd.print("    Jaringan    ");
  lcd.setCursor(0,1); lcd.print(" Wifi Terhubung ");  
}

void loop() {

  Blynk.run(); 

  float tinggiBadan = 194.0 - sonar.ping_cm();  

  float suhuTubuhC = mlx.readObjectTempC();
  float suhuSekitarC = mlx.readAmbientTempC();
  float suhuTubuhF = mlx.readObjectTempF();
  float suhuSekitarF = mlx.readAmbientTempF();

   suhuTubuhC = isnan(suhuTubuhC) ? 0 : suhuTubuhC;
   suhuSekitarC =  isnan(suhuSekitarC) ? 0 : suhuSekitarC;
   suhuTubuhF =  isnan(suhuTubuhF) ? 0 : suhuTubuhF;
   suhuSekitarF =  isnan(suhuSekitarF) ? 0 : suhuSekitarF;

  int PressureValue = analogRead(PressureSensorPin);
  mmhg = (PressureValue - 544) / 0.675;
  mmhg = (mmhg < 0) ? 0 : mmhg;

  float weight = loadCellData()/1000;
  float BMI = (weight) / pow(tinggiBadan/100 , 2);

  //=========================================================cetak
  if (millis() - cTime > 1000) {
     Serial.print("suhuTubuhC    :");  Serial.println(suhuTubuhC); 
     Serial.print("suhuSekitarC  :");  Serial.println(suhuSekitarC);
     Serial.print("suhuTubuhF    :");  Serial.println(suhuTubuhF); 
     Serial.print("suhuSekitarF  :");  Serial.println(suhuSekitarF);
     Serial.print("tekananDarah  :");  Serial.println(mmhg);
     Serial.print("Tinggi Badan  :");  Serial.println(tinggiBadan);
     Serial.print("Berat: "); Serial.print(weight); Serial.println(" gram");
     Serial.print("BMI  : "); Serial.print(BMI); Serial.println("");

     Blynk.virtualWrite(V3, suhuTubuhC);
     Blynk.virtualWrite(V4, tinggiBadan);
     Blynk.virtualWrite(V5, mmhg);
     Blynk.virtualWrite(V6, weight);
     Blynk.virtualWrite(V7, BMI);
     Blynk.virtualWrite(V9, PressureValue);

  lcd.setCursor(0,0); lcd.print("Suhu:"); lcd.print(suhuTubuhC); 
                      lcd.print("C Tensi:"); lcd.print(mmhg); lcd.print("  "); 
  lcd.setCursor(0,1); lcd.print("Tinggi : "); lcd.print(tinggiBadan); 
                      lcd.print("Cm  "); 
  lcd.setCursor(0,2); lcd.print("Berat Badan :  "); lcd.print(weight);
                      lcd.print("kg  "); 
  lcd.setCursor(0,3); lcd.print("BMI Index :  ");  lcd.print(BMI);
                      lcd.print("  "); 

    if (millis() - cTime1 > 3000){
       if(BMI >= 5.0 && BMI < 16){
          myDFPlayer.play(1); cTime1 = millis();
       } else if(BMI >= 16 && BMI < 18){
          myDFPlayer.play(2); cTime1 = millis();
       } else if(BMI >= 18 && BMI < 25){
          myDFPlayer.play(3); cTime1 = millis();
       } else if(BMI >= 25 && BMI < 27){
          myDFPlayer.play(4); cTime1 = millis();
       } else if(BMI >= 27){
          myDFPlayer.play(5); cTime1 = millis();
       }
    }
     Serial.println(""); cTime = millis();  
  }
}


BLYNK_WRITE(V1){
  int value = param.asInt();  
  if (value == 1) {
   digitalWrite(motorRelay,HIGH);
  } else {
   digitalWrite(motorRelay,LOW);
  }
}

void calibrate() {
  Serial.println("***");
  Serial.println("Start calibration:");
  Serial.println("Place the load cell an a level stable surface.");
  Serial.println("Remove any load applied to the load cell.");
  Serial.println("Send 't' from serial monitor to set the tare offset.");

  boolean _resume = false;
  while (_resume == false) {
    LoadCell.update();
    if (Serial.available() > 0) {
      if (Serial.available() > 0) {
        char inByte = Serial.read();
        if (inByte == 't') LoadCell.tareNoDelay();
      }
    }
    if (LoadCell.getTareStatus() == true) {
      Serial.println("Tare complete");
      _resume = true;
    }
  }

  Serial.println("Now, place your known mass on the loadcell.");
  Serial.println("Then send the weight of this mass (i.e. 100.0) from serial monitor.");

  float known_mass = 0;
  _resume = false;
  while (_resume == false) {
    LoadCell.update();
    if (Serial.available() > 0) {
      known_mass = Serial.parseFloat();
      if (known_mass != 0) {
        Serial.print("Known mass is: ");
        Serial.println(known_mass);
        _resume = true;
      }
    }
  }

  LoadCell.refreshDataSet(); //refresh the dataset to be sure that the known mass is measured correct
  float newCalibrationValue = LoadCell.getNewCalibration(known_mass); //get the new calibration value

  Serial.print("New calibration value has been set to: ");
  Serial.print(newCalibrationValue);
  Serial.println(", use this as calibration value (calFactor) in your project sketch.");
  Serial.print("Save this value to EEPROM adress ");
  Serial.print(calVal_eepromAdress);
  Serial.println("? y/n");

  _resume = false;
  while (_resume == false) {
    if (Serial.available() > 0) {
      char inByte = Serial.read();
      if (inByte == 'y') {
#if defined(ESP8266)|| defined(ESP32)
        EEPROM.begin(512);
#endif
        EEPROM.put(calVal_eepromAdress, newCalibrationValue);
#if defined(ESP8266)|| defined(ESP32)
        EEPROM.commit();
#endif
        EEPROM.get(calVal_eepromAdress, newCalibrationValue);
        Serial.print("Value ");
        Serial.print(newCalibrationValue);
        Serial.print(" saved to EEPROM address: ");
        Serial.println(calVal_eepromAdress);
        _resume = true;

      }
      else if (inByte == 'n') {
        Serial.println("Value not saved to EEPROM");
        _resume = true;
      }
    }
  }

  Serial.println("End calibration");
  Serial.println("***");
  Serial.println("To re-calibrate, send 'r' from serial monitor.");
  Serial.println("For manual edit of the calibration value, send 'c' from serial monitor.");
  Serial.println("***");
}

void changeSavedCalFactor() {
  float oldCalibrationValue = LoadCell.getCalFactor();
  boolean _resume = false;
  Serial.println("***");
  Serial.print("Current value is: ");
  Serial.println(oldCalibrationValue);
  Serial.println("Now, send the new value from serial monitor, i.e. 696.0");
  float newCalibrationValue;
  while (_resume == false) {
    if (Serial.available() > 0) {
      newCalibrationValue = Serial.parseFloat();
      if (newCalibrationValue != 0) {
        Serial.print("New calibration value is: ");
        Serial.println(newCalibrationValue);
        LoadCell.setCalFactor(newCalibrationValue);
        _resume = true;
      }
    }
  }
  _resume = false;
  Serial.print("Save this value to EEPROM adress ");
  Serial.print(calVal_eepromAdress);
  Serial.println("? y/n");
  while (_resume == false) {
    if (Serial.available() > 0) {
      char inByte = Serial.read();
      if (inByte == 'y') {
#if defined(ESP8266)|| defined(ESP32)
        EEPROM.begin(512);
#endif
        EEPROM.put(calVal_eepromAdress, newCalibrationValue);
#if defined(ESP8266)|| defined(ESP32)
        EEPROM.commit();
#endif
        EEPROM.get(calVal_eepromAdress, newCalibrationValue);
        Serial.print("Value ");
        Serial.print(newCalibrationValue);
        Serial.print(" saved to EEPROM address: ");
        Serial.println(calVal_eepromAdress);
        _resume = true;
      }
      else if (inByte == 'n') {
        Serial.println("Value not saved to EEPROM");
        _resume = true;
      }
    }
  }
  Serial.println("End change calibration value");
  Serial.println("***");
}
