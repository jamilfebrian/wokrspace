#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Adafruit_MLX90614.h>  
#include <MAX30100_PulseOximeter.h>
#define REPORTING_PERIOD_MS 5000
#include "MAX30100.h" //library sensor

LiquidCrystal_I2C lcd(0x27,16,2); //library lcd

MAX30100* pulseOxymeter;   //library

int cacah; //membuat variabel
int spo;
float bpm;
Adafruit_MLX90614 mlx = Adafruit_MLX90614();

float TargetC;
int dataadc;
int suhunya;
int bpmku;

PulseOximeter pox;

uint32_t tsLastReport = 0;
String hrData = "";
void onBeatDetected()
{
//    Serial.println("Beat!");
}

void setup() {
  Wire.begin();
  mlx.begin();
  Serial.begin(9600);
  lcd.begin();
  lcd.clear();

   //Serial.print("Initializing pulse oximeter..");
 
    if (!pox.begin()) {
        //Serial.println("FAILED");
        for(;;);
    } else {
        //Serial.println("SUCCESS");
    }
 
   pox.setIRLedCurrent(MAX30100_LED_CURR_11MA);
   pox.setOnBeatDetectedCallback(onBeatDetected);
 
}
   
void loop(){
 
   pox.update();
    
   if (millis() - tsLastReport > REPORTING_PERIOD_MS) {

       bpm = pox.getHeartRate();
       spo = pox.getSpO2();
       bpmku = bpm;
        
    tsLastReport = millis();
}

mlx.begin();
TargetC = mlx.readObjectTempC();
suhunya = TargetC;

lcd.setCursor(0,0);
lcd.print("T: ");
lcd.print(suhunya);
lcd.print(" C   ");

delay(1);
}
