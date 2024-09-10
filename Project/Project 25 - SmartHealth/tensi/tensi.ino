#define BLYNK_TEMPLATE_ID "TMPL61IxapWdA"
#define BLYNK_TEMPLATE_NAME "Project Testing"
#define BLYNK_AUTH_TOKEN "ICfMQqpE867-F7YvRUOehw5CPgQO8pGz"

#define BLYNK_PRINT Serial
#include <Arduino.h>
#include <Wire.h>
#include <SPI.h>
#include <SimpleTimer.h>
#include <LiquidCrystal_I2C.h>
#include <BlynkSimpleEsp32.h> // Ganti dengan library Blynk untuk ESP32

char ssid[] = "192.168.50.10";
char pass[] = "12345678";

LiquidCrystal_I2C lcd(0x27, 20, 4);

SimpleTimer timer;

int motor = 25; // Ubah pin D8 ke GPIO yang sesuai pada ESP32
int solenoid = 27; // Ubah pin 12 ke GPIO yang sesuai pada ESP32
int dataadc;
float mmhg;

void setup() {
  lcd.clear();
  lcd.init();
  lcd.noCursor();
  Serial.begin(9600);
  pinMode(motor, OUTPUT);
  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);
}

BLYNK_WRITE(V1) {
  int buttonState = param.asInt();
  if (buttonState == 1) {
    digitalWrite(motor, HIGH);
  } else {
    digitalWrite(motor, LOW);
  }
}

void loop() {
  Blynk.run();

  dataadc = analogRead(34); // Menggunakan pin analog di ESP32, misalnya GPIO 34
  mmhg = (dataadc - 250.43) / 0.2126;

  if (mmhg < 0) {
    mmhg = 0;
  }

  lcd.setCursor(0, 2);
  lcd.print("ADC/mmHg:");
  lcd.print(dataadc);
  lcd.print("/");
  lcd.print(mmhg);
  lcd.print("     ");

  Serial.print("ADC/mmHg:");
  Serial.print(dataadc);
  Serial.print("/");
  Serial.println(mmhg);

  // Mengirim data ADC dan tekanan darah aproksimasi ke Blynk
  Blynk.virtualWrite(V8, dataadc);
  Blynk.virtualWrite(V6, mmhg);

  delay(200);
}
