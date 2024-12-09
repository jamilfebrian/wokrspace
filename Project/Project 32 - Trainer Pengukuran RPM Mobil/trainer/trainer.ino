#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27,20,4);

#define SENSOR_PIN 11   
volatile int pulseCount = 0;
unsigned long lastTime = 0;
float wheelCircumference = 2.0;

void countPulse() {
  pulseCount++; 
}

void setup() {
  lcd.init();  lcd.backlight();  lcd.clear();
  Serial.begin(9600);         
  pinMode(SENSOR_PIN, INPUT); 
  attachInterrupt(digitalPinToInterrupt(SENSOR_PIN), countPulse, RISING); 
}

void loop() {
  unsigned long currentTime = millis();
  unsigned long elapsedTime = currentTime - lastTime;

  if (elapsedTime >= 1000) {
    noInterrupts(); 
    float speed = (pulseCount * wheelCircumference) / (elapsedTime / 1000.0); 
    pulseCount = 0; 
    interrupts(); 

    Serial.print("Kecepatan: ");
    Serial.print(speed * 3.6);
    Serial.println(" km/jam");

  lcd.setCursor(0,0);
  lcd.print("Speed: ");
  lcd.print(speed * 3.6);
  lcd.print(" km/jam");
  lcd.setCursor(0,1);
  lcd.print("Ywrobot Arduino!");

    lastTime = currentTime;
  }
}
