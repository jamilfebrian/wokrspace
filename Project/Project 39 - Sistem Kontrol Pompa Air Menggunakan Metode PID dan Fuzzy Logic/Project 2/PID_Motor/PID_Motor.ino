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

float Kp = 1.0;
float Ki = 1.5;
float Kd = 0;

float error = 0;
float previousError = 0;
float integral = 0;
float derivative = 0;
float output = 0;

unsigned long previousMillis = 0;

LiquidCrystal_I2C lcd(0x27,16,2);

void setup()
{
  Serial.begin(9600);
  espSerial.begin(9600);

  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);

  pinMode(ENCODER_A, INPUT_PULLUP);
  pinMode(ENCODER_B, INPUT_PULLUP);

  lcd.init();  lcd.backlight(); lcd.clear();

  attachInterrupt(digitalPinToInterrupt(ENCODER_A), readEncoder, RISING);

}

void loop()
{
  if (millis() - previousMillis >= 100)
  {
    noInterrupts();
    long pulses = encoderCount;
    encoderCount = 0;
    interrupts();

    float PPR = 341.2; 

    rpm = (pulses * 600.0) / PPR;

    // PID
    error = setpoint - rpm;
    integral += error * 0.1;
    derivative = (error - previousError) / 0.1;
    output = Kp * error +
             Ki * integral +
             Kd * derivative;
    previousError = error;

    if (output > 255) output = 255;
    if (output < 0) output = 0;

    analogWrite(IN1, output);
    analogWrite(IN2, LOW);

    String kirimData = String(setpoint) + "|" + String(rpm) + "|" + String(output) ;
    espSerial.println(kirimData);

    Serial.print("Setpoint : ");
    Serial.print(setpoint);

    Serial.print(" RPM   Actual : ");
    Serial.print(rpm);

    Serial.print(" PWM : ");
    Serial.println(output);
    // Serial.println(kirimData);

    lcd.setCursor(0,0);
    lcd.print(" PWM : ");
    lcd.print(output);
    lcd.print("  ");
    
    lcd.setCursor(0,1);
    lcd.print(" RPM : ");
    lcd.print(rpm);
    lcd.print("  ");
    previousMillis = millis();
  }
}

void readEncoder()
{
  encoderCount++;
}