#define WATER_LEVEL A0
#define ANALOG_RAIN_SENSOR A1
#define DIGITAL_RAIN_SENSOR 2

#define BUZZER_PIN 8
#define LED_STATUS 9
#define LED_EMERGENCY_PIN 10

#include <avr/sleep.h>

volatile int water_level, rain_value;

void setup() {
  Serial.begin(9600);
  
  pinMode(WATER_LEVEL, INPUT);
  pinMode(ANALOG_RAIN_SENSOR, INPUT);
  pinMode(DIGITAL_RAIN_SENSOR, INPUT);
  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(LED_STATUS, OUTPUT);
  pinMode(LED_EMERGENCY_PIN, OUTPUT);


  attachInterrupt(digitalPinToInterrupt(DIGITAL_RAIN_SENSOR), wakeUp, LOW); 
  
}

void loop() {
  bool rainState = digitalRead(DIGITAL_RAIN_SENSOR);

  if(rainState){
    Serial.print("Hujan Tidak Terdeteksi, Memasuki Mode Sleep");
    digitalWrite(LED_STATUS, LOW); delay(500);
    enterSleepMode();
  }else{
    Serial.print("Hujan Terdeteksi");
  }
}

void enterSleepMode() {
  set_sleep_mode(SLEEP_MODE_PWR_DOWN); 
  sleep_enable(); 
  sleep_cpu(); 
  sleep_disable(); 
}

void wakeUp() {
  water_level = analogRead(WATER_LEVEL);
  rain_value = analogRead(ANALOG_RAIN_SENSOR);
  digitalWrite(LED_STATUS, HIGH);

  Serial.print(water_level);
  Serial.print(" # ");
  Serial.println(rain_value);
  delay(100);

  if(water_level > 500){
    digitalWrite(BUZZER_PIN, HIGH);
    digitalWrite(LED_EMERGENCY_PIN, HIGH);
    delay(50000);
    digitalWrite(BUZZER_PIN, LOW);
    digitalWrite(LED_EMERGENCY_PIN, LOW);
    delay(50000);
  } else {
    digitalWrite(BUZZER_PIN, LOW);
    digitalWrite(LED_EMERGENCY_PIN, LOW);
  }
}
