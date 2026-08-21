#define TRIG_PIN 9
#define ECHO_PIN 8
#define SELENOID 5

#define ENB 10
#define IN3 12
#define IN4 11

#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);

int pwmOutput;

float distance;
float PanjangTangki = 0;
float LebarTangki = 0;

float readUltrasonic() {
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);

  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);

  digitalWrite(TRIG_PIN, LOW);

  long duration = pulseIn(ECHO_PIN, HIGH);
  return duration * 0.034 / 2.0;
}

//================= FUZZY INPUT =================

float sangatRendah(float x) {
  if (x >= 18)
    return 1;
  else if (x > 15)
    return (x - 15) / 3.0;
  else
    return 0;
}

float rendah(float x) {
  if (x <= 12 || x >= 18)
    return 0;
  else if (x < 15)
    return (x - 12) / 3.0;
  else
    return (18 - x) / 3.0;
}

float sedang(float x) {
  if (x <= 9 || x >= 15)
    return 0;
  else if (x < 12)
    return (x - 9) / 3.0;
  else
    return (15 - x) / 3.0;
}

float tinggi(float x) {
  if (x <= 6 || x >= 12)
    return 0;
  else if (x < 9)
    return (x - 6) / 3.0;
  else
    return (12 - x) / 3.0;
}

float sangatTinggi(float x) {
  if (x <= 5)
    return 1;
  else if (x < 8)
    return (8 - x) / 3.0;
  else
    return 0;
}

//=================================================

void setup() {

  Serial.begin(9600);

  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);

  pinMode(SELENOID, OUTPUT);
  digitalWrite(SELENOID, LOW);

  pinMode(ENB, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);

  // arah putaran pompa
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);

  lcd.init(); lcd.backlight(); lcd.clear();
}

void loop() {

  distance = readUltrasonic();

  if (distance > 20)
    distance = 20;

  if (distance < 5)
    distance = 5;
  
  float TinggiTangki = 20 - distance;
  float VolumeAir = PanjangTangki *LebarTangki * TinggiTangki;

  float SR = sangatRendah(distance);
  float R = rendah(distance);
  float S = sedang(distance);
  float T = tinggi(distance);
  float ST = sangatTinggi(distance);

  float pwmSR = SR * 255;   // sangat cepat
  float pwmR = R * 200;    // cepat
  float pwmS = S * 150;    // sedang
  float pwmT = T * 80;     // lambat
  float pwmST = ST * 0;    // mati

  float numerator =
      pwmSR +
      pwmR +
      pwmS +
      pwmT +
      pwmST;

  float denominator =
      SR +
      R +
      S +
      T +
      ST;

  if (denominator != 0)
    pwmOutput = numerator / denominator;
  else
    pwmOutput = 0;

  if (distance <= 5){
    pwmOutput = 0;
    digitalWrite(SELENOID, HIGH);
  } else {
    digitalWrite(SELENOID, LOW);
  }
    
  analogWrite(ENB, pwmOutput);

  lcd.setCursor(0, 0);
  lcd.print("V="); lcd.print(VolumeAir); lcd.print("   ");
  lcd.setCursor(0, 1);
  lcd.print("PWM="); lcd.print(pwmOutput); lcd.print("   ");

  Serial.print("Jarak = ");
  Serial.print(distance);
  Serial.print(" cm");

  Serial.print(" | SR=");
  Serial.print(SR, 2);

  Serial.print(" R=");
  Serial.print(R, 2);

  Serial.print(" S=");
  Serial.print(S, 2);

  Serial.print(" T=");
  Serial.print(T, 2);

  Serial.print(" ST=");
  Serial.print(ST, 2);

  Serial.print(" | PWM = ");
  Serial.println(pwmOutput);

  delay(500);
}