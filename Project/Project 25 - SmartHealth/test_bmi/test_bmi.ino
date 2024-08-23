#define BLYNK_TEMPLATE_ID "TMPL61IxapWdA"
#define BLYNK_TEMPLATE_NAME "Project Testing"
#define BLYNK_AUTH_TOKEN "ICfMQqpE867-F7YvRUOehw5CPgQO8pGz"
#define BLYNK_PRINT Serial

#include <HX711_ADC.h>
#include <EEPROM.h>
#include <NewPing.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <BlynkSimpleEsp32.h>
#include <Adafruit_MLX90614.h>
#include "DFRobotDFPlayerMini.h"

// Definisikan pin sesuai dengan kebutuhan
#define TRIGGER_PIN 12
#define ECHO_PIN 14
#define MAX_DISTANCE 200
const int HX711_dout = 18;
const int HX711_sck = 23;
HardwareSerial mySerial(1); // ESP32 memiliki lebih banyak hardware serial
DFRobotDFPlayerMini myDFPlayer;

HX711_ADC LoadCell(HX711_dout, HX711_sck);
NewPing ultrasonic(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE);
LiquidCrystal_I2C lcd(0x27, 20, 4);
char ssid[] = "192.168.50.10";
char pass[] = "12345678";
const int calVal_calVal_eepromAdress = 0;
unsigned long t = 0;
unsigned long lastBMICheckTime = 0;
float lastBMI = 0;

void setup() {
  Serial.begin(9600);
  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);
  delay(10);
  Serial.println();
  Serial.println("Starting...");
  float calibrationValue;
  calibrationValue = 22.41;
  LoadCell.begin();
  unsigned long stabilizingtime = 2000;
  boolean _tare = true;
  LoadCell.start(stabilizingtime, _tare);
  if (LoadCell.getTareTimeoutFlag()) {
    Serial.println("Timeout, check MCU>HX711 wiring and pin designations");
  } else {
    LoadCell.setCalFactor(calibrationValue);
    Serial.println("Startup is complete");
  }
  while (!LoadCell.update());
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

  mySerial.begin(9600, SERIAL_8N1, 16, 17); // Pin TX dan RX untuk ESP32
  if (!myDFPlayer.begin(mySerial)) {
    Serial.println("Unable to begin DFPlayer");
    while (true);
  }

  myDFPlayer.setTimeOut(500);
  myDFPlayer.volume(20);

  delay(2000);
}

void loop() {
  Blynk.run();
  lcd.clear();
  static boolean newDataReady = 0;
  const int serialPrintInterval = 500;
  if (LoadCell.update()) newDataReady = true;
  if (newDataReady) {
    if (millis() > t + serialPrintInterval) {
      float currentBMI = calculateBMI();
      Serial.print("Load_cell output val: ");
      Serial.println(currentBMI);
      checkBMIChange(currentBMI);
      newDataReady = 0;
      t = millis();
    }
  }
  if (Serial.available() > 0) {
    char inByte = Serial.read();
    if (inByte == 't') LoadCell.tareNoDelay();
  }
  if (LoadCell.getTareStatus() == true) {
    Serial.println("Tare complete");
  }

  int landasan = 192;
  int tinggi = ultrasonic.ping_cm();
  int tinggibadan = landasan - tinggi;
  int beratbadan = LoadCell.getData() / 1000;

  float tinggiMeter = tinggibadan / 100.0;
  float bmi = beratbadan / (tinggiMeter * tinggiMeter);

  lcd.setCursor(0, 0);
  lcd.print("TB/BB:");
  lcd.print(tinggibadan);
  lcd.print("/");
  lcd.print(beratbadan);

  lcd.setCursor(0, 1);
  lcd.print("BMI:");
  lcd.print(bmi);

  Serial.print("TB(cm)/BB(kg): ");
  Serial.print(tinggibadan);
  Serial.print("/");
  Serial.print(beratbadan);
  Serial.print("  ");
  Serial.print("BMI: ");
  Serial.println(bmi);
  Blynk.virtualWrite(V4, tinggibadan);
  Blynk.virtualWrite(V6, beratbadan);
  Blynk.virtualWrite(V7, bmi);
}

float calculateBMI() {
  int beratbadan = LoadCell.getData() / 1000;
  float tinggiMeter = (192 - ultrasonic.ping_cm()) / 100.0;
  return beratbadan / (tinggiMeter * tinggiMeter);
}

void checkBMIChange(float currentBMI) {
  if (currentBMI == lastBMI) {
    if (millis() - lastBMICheckTime > 2000) {  // Check every 5 seconds
      playSoundBasedOnBMI(currentBMI);
    }
  } else {
    lastBMI = currentBMI;
    lastBMICheckTime = millis();
  }
}

void playSoundBasedOnBMI(float bmi) {
  if (bmi >= 5 && bmi < 16) {
    playSound(1);
  } else if (bmi >= 16 && bmi < 18) {
    playSound(2);
  } else if (bmi >= 18 && bmi < 25) {
    playSound(3);
  } else if (bmi >= 25 && bmi < 27) {
    playSound(4);
  } else if (bmi >= 27) {
    playSound(5);
  }
}

void playSound(int soundNumber) {
  myDFPlayer.play(soundNumber);
  delay(2500);
}
