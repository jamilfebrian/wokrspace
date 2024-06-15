#include <NewPing.h>
#include <LiquidCrystal_I2C.h>

#define SAKLAR 13
#define BUZZER 3

LiquidCrystal_I2C lcd(0x27,16,2); 
NewPing sonar(7, 6, 100);

void setup() {
  lcd.init();  lcd.backlight();  lcd.clear();
  Serial.begin(115200);

  pinMode(SAKLAR, INPUT_PULLUP);
  pinMode(BUZZER, OUTPUT);
}

void loop() {
  long range = sonar.ping_cm();

  if(!digitalRead(SAKLAR)){
    digitalWrite(BUZZER, HIGH);
  } else {
    digitalWrite(BUZZER, LOW);
  }
 
  lcd.setCursor(0,0);
  lcd.print("Range : ");
  lcd.print(range);
  lcd.print("   ");
  delay(100);
}
