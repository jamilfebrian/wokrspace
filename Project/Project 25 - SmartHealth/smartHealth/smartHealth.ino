#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
#include <Adafruit_MLX90614.h>
#include <NewPing.h>

#define PressureSensorPin 26
#define motorRelay 25
#define selenoid 27

#define TRIGGER_PIN  12  
#define ECHO_PIN     14
#define MAX_DISTANCE 200

Adafruit_MLX90614 mlx = Adafruit_MLX90614();
LiquidCrystal_I2C lcd(0x27,20,4);  
NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE); 

void setup() {
  Serial.begin(115200);

  lcd.init(); lcd.backlight(); lcd.clear();
  pinMode(PressureSensorPin, INPUT);
  pinMode(motorRelay, OUTPUT);
  pinMode(selenoid, OUTPUT);

  digitalWrite(selenoid, HIGH);
  digitalWrite(motorRelay, HIGH);

  mlx.begin(0x5A);
  
}

void loop() {

  int PressureValue = analogRead(PressureSensorPin);
  float tinggiBadan = 192.0 - sonar.ping_cm();  

  float suhuTubuhC = mlx.readObjectTempC();
  float suhuSekitarC = mlx.readAmbientTempC();
  float suhuTubuhF = mlx.readObjectTempF();
  float suhuSekitarF = mlx.readAmbientTempF();

  float tekanan = (PressureValue / 4095.0) * 5000000;  // Konversi nilai ADC ke tekanan dalam Pascal
        tekanan = tekanan / 133.322; //konversi nilai pascal ke mmHg

  //=========================================================cetak
   Serial.print("suhuTubuhC    :");  Serial.println(suhuTubuhC); 
   Serial.print("suhuSekitarC  :");  Serial.println(suhuSekitarC);
   Serial.print("suhuTubuhF    :");  Serial.println(suhuTubuhF); 
   Serial.print("suhuSekitarF  :");  Serial.println(suhuSekitarF);
   Serial.print("tekananDarah  :");  Serial.println(PressureValue);
   Serial.print("Tinggi Badan  :");  Serial.println(tinggiBadan);
   Serial.println(""); delay(1000);
  

}
