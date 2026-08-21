#define IN1 10
#define IN2 9

#define ENCODER_A 2
#define ENCODER_B 3

#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <SoftwareSerial.h>

SoftwareSerial espSerial(7, 8); // RX, TX

volatile long encoderCount = 0;

float rpm = 0;
float setpoint = 200;

unsigned long previousMillis = 0;

LiquidCrystal_I2C lcd(0x27,16,2);

void setup(){
  Serial.begin(9600);
  espSerial.begin(9600);

  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);

  pinMode(ENCODER_A, INPUT_PULLUP);
  pinMode(ENCODER_B, INPUT_PULLUP);

  lcd.init();
  lcd.backlight();
  lcd.clear();

  attachInterrupt(digitalPinToInterrupt(ENCODER_A), readEncoder, RISING);
}

void loop(){
  if (millis() - previousMillis >= 100)
  {
    noInterrupts();
    long pulses = encoderCount;
    encoderCount = 0;
    interrupts();

    float PPR = 341.2;
    rpm = (pulses * 600.0) / PPR;

    float error = setpoint - rpm;
    int output = fuzzyControl(error);

    analogWrite(IN1, output);
    analogWrite(IN2, LOW);

    Serial.print("Setpoint : ");
    Serial.print(setpoint);

    Serial.print(" RPM Actual : ");
    Serial.print(rpm);

    Serial.print(" Error : ");
    Serial.print(error);

    Serial.print(" PWM : ");
    Serial.println(output);

    String kirimData = String(setpoint) + "|" + String(rpm) + "|" + String(output) ;
    espSerial.println(kirimData);

    lcd.setCursor(0,0);
    lcd.print("PWM : ");
    lcd.print(output);
    lcd.print("   ");

    lcd.setCursor(0,1);
    lcd.print("RPM : ");
    lcd.print(rpm);
    lcd.print("   ");

    previousMillis = millis();
  }
}

void readEncoder(){
  encoderCount++;
}

// FUZZY CONTROLLER 5 LEVEL
int fuzzyControl(float error){
  int pwm;

  if (error <= 0){
    pwm = 0;
  }

  else if (error <= 20){
    pwm = 70;
  }

  else if (error <= 40){
    pwm = 120;
  }

  else if (error <= 60){
    pwm = 180;
  }

  else{
    pwm = 255;
  }

  return pwm;
}