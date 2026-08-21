#define TRIG_PIN 9
#define ECHO_PIN 8
#define SELENOID 5

#define ENB 10
#define IN3 12
#define IN4 11

#include <Wire.h>
#include <LiquidCrystal_I2C.h>

float Kp = 18;
float Ki = 0;
float Kd = 1;

float setpoint = 6;

float error;
float lastError = 0;
float integral = 0;
float derivative;
float output;

unsigned long lastTime;
float dt;

LiquidCrystal_I2C lcd(0x27, 16, 2);

void setup() {

  pinMode(A3, INPUT);

  Serial.begin(9600);

  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);

  pinMode(SELENOID, OUTPUT);
  digitalWrite(SELENOID, LOW);

  pinMode(ENB, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);

  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);

  lcd.init(); lcd.backlight(); lcd.clear();

  lastTime = millis();
}

float bacaJarak() {

  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);

  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);

  digitalWrite(TRIG_PIN, LOW);

  long duration = pulseIn(ECHO_PIN, HIGH);
  float distance = duration * 0.034 / 2;
  return distance;
}

void loop() {

  unsigned long now = millis();
  dt = (now - lastTime) / 1000.0;
  lastTime = now;

  float distance = bacaJarak();

  if (distance > 20)
    distance = 20;

  if (distance < 0)
    distance = 0;

  // ================= PID =================

  error = distance - setpoint;
  integral += error * dt;
  derivative = (error - lastError) / dt;
  output = (Kp * error) +
           (Ki * integral) +
           (Kd * derivative);
  lastError = error;

  // Batasi PWM
  if (output > 255)
    output = 255;

  if (output < 0)
    output = 0;

  if (distance <= setpoint) {
    output = 0;
    digitalWrite(SELENOID, HIGH);
  } else {
    digitalWrite(SELENOID, LOW);
  }

  // Kirim PWM ke pompa
  analogWrite(ENB, output);

  lcd.setCursor(0, 0);
  lcd.print("Jarak Air: ");
  lcd.print(distance);
  lcd.print(" cm");

  lcd.setCursor(0, 1);
  lcd.print("PWM: ");
  lcd.print(output);
  lcd.print(" | Error: ");
  lcd.println(error);

  Serial.print(distance); Serial.print(" | "); Serial.print(output); Serial.print(" | "); Serial.println(error);

  delay(100);
}