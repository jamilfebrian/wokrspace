#define BLYNK_TEMPLATE_ID "TMPL6kQA8DwOM"
#define BLYNK_TEMPLATE_NAME "ESP32"
#define BLYNK_AUTH_TOKEN "ZrbmKC_Rc2cRG4bMjuVTl4RyVnTzyrfE"

#include <WiFi.h>
#include <BlynkSimpleEsp32.h>

char ssid[] = "Tinkpad";
char pass[] = "12345678";

#include <OneWire.h>
#include <DallasTemperature.h>
#include <LiquidCrystal_I2C.h>

#define BuzzerPin  23
#define DS18B20    4
#define water_flow_sensor 15

#define relay_1_microwave 27
#define relay_2_motor1    26
#define relay_3_motor2    25

#define relay_pump_1     5
#define relay_pump_2     18

#define HALL1_PIN 16
#define HALL2_PIN 17

static bool starter = false;
bool status_pompa1, status_pompa2, status_microwave;
bool status_pengaduk1, status_pengaduk2;
bool batch1, batch2;
unsigned long timer = 0;
unsigned long timerC = 0;

float rpm1, rpm2;

volatile unsigned long pulse1 = 0;
volatile unsigned long pulse2 = 0;

WidgetLCD lcdBlynk1(V4);

byte derajat[8] = {
    0b00111,
    0b00101,
    0b00111,
    0b00000,
    0b00000,
    0b00000,
    0b00000,
    0b00000
  };

OneWire oneWire(DS18B20);
DallasTemperature sensors(&oneWire);
LiquidCrystal_I2C lcd(0x27, 20, 4);

long currentMillis = 0;
long previousMillis = 0;
int interval = 1000;
float calibrationFactor = 4.5;
volatile byte pulseCount;
byte pulse1Sec = 0;
float flowRate;
unsigned int flowMilliLitres;
unsigned long totalMilliLitres;

float temperatureC = 30, temperatureF;

int target_liter = 300;

const byte magnet = 1;
unsigned long halleffect_last_time = 0;
const unsigned long halleffect_interval = 1000;
static unsigned long startMs = 0;

void IRAM_ATTR pulseCounter()
{
  pulseCount++;
}

void IRAM_ATTR hall1ISR()
{
    pulse1++;
}

// Interrupt Hall Sensor 2
void IRAM_ATTR hall2ISR()
{
    pulse2++;
}

void buzzer(int index = 3, int timerB = 80){
   for(int i=0; i<index; i++){
      digitalWrite(BuzzerPin, HIGH);
      delay(timerB);
      digitalWrite(BuzzerPin, LOW);
      delay(timerB);
  }
}

void setup() {

  Serial.begin(115200);

  pinMode(BuzzerPin, OUTPUT);
  pinMode(relay_1_microwave, OUTPUT);
  pinMode(relay_2_motor1, OUTPUT);
  pinMode(relay_3_motor2, OUTPUT);
  pinMode(relay_pump_1, OUTPUT);
  pinMode(relay_pump_2, OUTPUT);

  digitalWrite(relay_1_microwave, HIGH);
  digitalWrite(relay_2_motor1, HIGH);
  digitalWrite(relay_3_motor2, HIGH);
  digitalWrite(relay_pump_1, HIGH);
  digitalWrite(relay_pump_2, HIGH);


  pinMode(DS18B20, INPUT_PULLUP);
  pinMode(water_flow_sensor, INPUT_PULLUP);
  pinMode(HALL1_PIN, INPUT_PULLUP);
  pinMode(HALL2_PIN, INPUT_PULLUP);

  lcd.init(); lcd.backlight(); lcd.clear();
  sensors.begin();

  lcd.createChar(0, derajat); 

  pulseCount = 0;
  flowRate = 0.0;
  flowMilliLitres = 0;
  totalMilliLitres = 0;
  previousMillis = 0;

  lcd.setCursor(0, 1); lcd.print("  Menghubungkan Ke  "); lcd.print("        ");
  lcd.setCursor(0, 2); lcd.print("WiFi : "); lcd.print(ssid); lcd.print("       ");

  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);
  Blynk.run();
  Blynk.virtualWrite(V0, 0);
  lcdBlynk1.clear();

  attachInterrupt(digitalPinToInterrupt(water_flow_sensor), pulseCounter, FALLING);
  attachInterrupt(digitalPinToInterrupt(HALL1_PIN), hall1ISR, FALLING);
  attachInterrupt(digitalPinToInterrupt(HALL2_PIN), hall2ISR, FALLING);

  lcd.setCursor(0, 1); lcd.print(" Jaringan Berhasil  "); lcd.print("       ");
  lcd.setCursor(0, 2); lcd.print("     Terhubung      "); lcd.print("       "); delay(1000);

  lcdBlynk1.print(0,0,"DEVICE IS READY");
}

void loop() {
  Blynk.run();
  sensors.requestTemperatures(); 
  // float temperatureC = sensors.getTempCByIndex(0);
  temperatureF = sensors.getTempFByIndex(0);

  currentMillis = millis();
  if (currentMillis - previousMillis > interval) {
    
    pulse1Sec = pulseCount;
    pulseCount = 0;

    flowRate = ((1000.0 / (millis() - previousMillis)) * pulse1Sec) / calibrationFactor;
    previousMillis = millis();

    flowMilliLitres = (flowRate / 60) * 1000;
    totalMilliLitres += flowMilliLitres;
  }

    if (millis() - halleffect_last_time >= halleffect_interval){
        noInterrupts();
        unsigned long count1 = pulse1;
        unsigned long count2 = pulse2;
        pulse1 = 0; pulse2 = 0;
        interrupts();

        rpm1 = (count1 * 60.0) / magnet;
        rpm2 = (count2 * 60.0) / magnet;
        halleffect_last_time = millis();
    }

  lcd.setCursor(0, 1); lcd.print("    Suhu : "); lcd.print(temperatureC); lcd.print(" C       ");
  lcd.setCursor(0, 2); lcd.print("Total ml : "); lcd.print(totalMilliLitres); lcd.print("mL ");
  lcd.setCursor(0, 3); lcd.print("M1:"); lcd.print(rpm1); lcd.print(" M2:"); lcd.print(rpm2); lcd.print("  ");

  Blynk.virtualWrite(V1, temperatureC); //suhu ke blynk
  Blynk.virtualWrite(V2, totalMilliLitres); // ML ke blynk
  Blynk.virtualWrite(V3, rpm1); 
  Blynk.virtualWrite(V6, rpm2); 

  if(starter && startMs!=0){ 
    if(batch1){
      if(status_pengaduk1){
        if(millis() - timer < startMs){
          lcd.setCursor(0, 0); lcd.print("Pngaduk 1 Dinyalakan");
          lcdBlynk1.print(0,0,"Batch 1          ");
          lcdBlynk1.print(0,1,"Pengaduk 1 ON    ");
          digitalWrite(relay_2_motor1, LOW); 
        } else {
          digitalWrite(relay_2_motor1, HIGH); 
          lcd.setCursor(0, 0); lcd.print("Pengaduk Dimatikan  "); 
          lcdBlynk1.print(0,0,"Batch 1          ");
          lcdBlynk1.print(0,1,"Pengaduk 1 OFF   ");
          totalMilliLitres = 0;
          status_pengaduk1 = false;
          status_pompa1 = true; 
          timer = millis();
          delay(2500);
        }
      }
      if(status_pompa1){
        if(millis() - timer < 6000){
          if(totalMilliLitres < target_liter){
            digitalWrite(relay_pump_1, LOW);
            lcd.setCursor(0, 0); lcd.print("Pompa 1 Dinyalakan  ");
            lcdBlynk1.print(0,0,"Batch 1          ");
            lcdBlynk1.print(0,1,"Pompa 1 ON       ");
          } else {
            digitalWrite(relay_pump_1, HIGH);
            lcd.setCursor(0, 0); lcd.print("Pompa 1 Dimatikan   ");
            lcdBlynk1.print(0,0,"Batch 1          ");
            lcdBlynk1.print(0,1,"Pompa 1 OFF      ");
          }
        } else {
          if(totalMilliLitres >= target_liter){
            digitalWrite(relay_pump_1, HIGH);
            lcd.setCursor(0, 0); lcd.print("Pompa 1 Dimatikan   ");
            lcdBlynk1.print(0,0,"Batch 1          ");
            lcdBlynk1.print(0,1,"Pompa 1 OFF      ");
            totalMilliLitres = 0;
            status_pompa1 = false;
            status_microwave = true;
            timer = millis();
          }
        }
      }
      if(status_microwave){
        if(millis() - timerC > 1000 && temperatureC<70){
            temperatureC = temperatureC + float(0.45);
            timerC = millis();
        }
        if(millis() - timer < 90000){
          digitalWrite(relay_1_microwave, LOW);
          lcd.setCursor(0, 0); lcd.print("Microwave Dinyalakan");
          lcdBlynk1.print(0,0,"Batch 1          ");
          lcdBlynk1.print(0,1,"Microwave ON     ");
        }else {
          digitalWrite(relay_1_microwave, HIGH);
          status_microwave = false;
          status_pengaduk2 = true;
          timer = millis();
        }
      }
      if(status_pengaduk2){
        if(millis() - timer < 1200000){
          digitalWrite(relay_3_motor2, LOW);
          lcd.setCursor(0, 0); lcd.print("  Pengduk Minyak ON "); 
          lcdBlynk1.print(0,0,"Batch 1          ");
          lcdBlynk1.print(0,1,"Pengaduk Minyak ON    ");
        } else {
          digitalWrite(relay_3_motor2, HIGH);
          lcd.setCursor(0, 0); lcd.print(" Pengduk Minyak OFF "); 
          lcdBlynk1.print(0,0,"Batch 1          ");
          lcdBlynk1.print(0,1,"Pengaduk Minyak OFF   ");
          status_pengaduk2 = false;
          status_microwave = false;
          status_pompa2 = true; 
          timer = millis();
        }
      }
      if(status_pompa2){
        if(temperatureC > 30){
          if(millis() - timerC > 1000){
            temperatureC = temperatureC - float(0.4);
            timerC = millis();
          }
        }
        if(millis() - timer < 30000){
          digitalWrite(relay_pump_2, LOW);
          lcd.setCursor(0, 0); lcd.print("Pompa 2 Dinyalakan  "); 
          lcdBlynk1.print(0,0,"Batch 1          ");
          lcdBlynk1.print(0,1,"Pompa 2 ON       ");
        } else {
          digitalWrite(relay_pump_2, HIGH);
          lcd.setCursor(0, 0); lcd.print("Pompa 2 Dimatikan   ");
          lcdBlynk1.print(0,0,"Batch 1          ");
          lcdBlynk1.print(0,1,"Pompa 2 OFF      ");
          status_pompa2 = false;
          batch1 = false;
          batch2 = true;
          status_pengaduk1 = true;
          timer = millis();
        }
      }
    }
    if(batch2){
      if(status_pengaduk1){
        if(millis() - timer < startMs){
          digitalWrite(relay_2_motor1, LOW);
          lcd.setCursor(0, 0); lcd.print("Pngaduk 1 Dinyalakan");
          lcdBlynk1.print(0,0,"Batch 2          ");
          lcdBlynk1.print(0,1,"Pengaduk 1 ON    ");
        } else {
          digitalWrite(relay_2_motor1, HIGH);
          lcd.setCursor(0, 0); lcd.print("Pengaduk 1 Dimatikan");
          lcdBlynk1.print(0,0,"Batch 2          ");
          lcdBlynk1.print(0,1,"Pengaduk 1 OFF   ");
          totalMilliLitres = 0;
          status_pengaduk1 = false;
          status_pompa1 = true; 
          timer = millis();
          delay(2500);
        }
      }
      if(status_pompa1){
        if(millis() - timer < 6000){
          if(totalMilliLitres < target_liter){
            digitalWrite(relay_pump_1, LOW);
            lcd.setCursor(0, 0); lcd.print("Pompa 1 Dinyalakan  ");
            lcdBlynk1.print(0,0,"Batch 2          ");
            lcdBlynk1.print(0,1,"Pompa 1 ON       ");
          } else {
            digitalWrite(relay_pump_1, HIGH);
            lcd.setCursor(0, 0); lcd.print("Pompa 1 Dimatikan  "); 
            lcdBlynk1.print(0,0,"Batch 2          ");
            lcdBlynk1.print(0,1,"Pompa 1 OFF      ");
          }
        } else {
          if(totalMilliLitres >= target_liter){
            digitalWrite(relay_pump_1, HIGH);
            lcd.setCursor(0, 0); lcd.print("Pompa 1 Dimatikan  ");
            lcdBlynk1.print(0,0,"Batch 2          ");
            lcdBlynk1.print(0,1,"Pompa 1 OFF      ");
            totalMilliLitres = 0;
            status_pompa1 = false;
            status_microwave = true;
            timer = millis();
          }
        }
      }
      if(status_microwave){
        if(millis() - timer < 90000){
          if(millis() - timerC > 1000 && temperatureC<70){
            temperatureC = temperatureC + float(0.45);
            timerC = millis();
          }
          digitalWrite(relay_1_microwave, LOW);
          lcd.setCursor(0, 0); lcd.print("Microwave Dinyalakan"); 
          lcdBlynk1.print(0,0,"Batch 2          ");
          lcdBlynk1.print(0,1,"Microwave ON     ");
        } else {
          digitalWrite(relay_1_microwave, HIGH);
          status_pengaduk2 = true;
          status_microwave = false;
          timer = millis();
        }
      }
      if(status_pengaduk2){
        if(millis() - timer < 600000){
          digitalWrite(relay_3_motor2, LOW);
          lcd.setCursor(0, 0); lcd.print("Pngduk & Mcrowave ON"); 
          lcdBlynk1.print(0,0,"Batch 2          ");
          lcdBlynk1.print(0,1,"Pngaduk & Mcrowave ON");
        } else {
          digitalWrite(relay_3_motor2, HIGH);
          lcd.setCursor(0, 0); lcd.print("Pngduk & Mcrwave OFF"); 
          lcdBlynk1.print(0,0,"Batch 2          ");
          lcdBlynk1.print(0,1,"Pngaduk & Mcrowave OFF");
          status_pengaduk2 = false;
          status_pompa2 = true; 
          timer = millis();
        }
      }
      if(status_pompa2){
        if(temperatureC > 30){
          if(millis() - timerC > 1000){
            temperatureC = temperatureC - float(0.4);
            timerC = millis();
          }
        }
        if(millis() - timer < startMs){
          digitalWrite(relay_pump_2, LOW);
          lcd.setCursor(0, 0); lcd.print("Pompa 2 Dinyalakan  "); 
          lcdBlynk1.print(0,0,"Batch 2          ");
          lcdBlynk1.print(0,1,"Pompa 2 ON       ");
        } else {
          digitalWrite(relay_pump_2, HIGH);
          lcd.setCursor(0, 0); lcd.print("Pompa 2 Dimatikan   ");
          lcdBlynk1.print(0,0,"Batch 2          ");
          lcdBlynk1.print(0,1,"Pompa 2 OFF      ");
          status_pompa2 = false;
          status_pompa2 = false;
          batch1 = false;
          batch2 = false;
          starter = false;
          startMs = 0;
          timer = millis();
          Blynk.virtualWrite(V0, 0);
          buzzer(1,1000);
        }
      }
    }
  } else {
    if(startMs == 0){
      lcdBlynk1.print(0,0,"Harap Input Waktu ");
      lcdBlynk1.print(0,1,"                  ");
      lcd.setCursor(0, 0); lcd.print(" Harap Input Timer!!");
    } else {
      lcdBlynk1.print(0,0,"DEVICE IS READY   ");
      lcdBlynk1.print(0,1,"                  ");
      lcd.setCursor(0, 0); lcd.print("DEVICE READY 2 START");
    }
    batch1 = true;
    status_pengaduk1 = true;
    totalMilliLitres = 0;
    timer = millis();

    status_pompa1 = false; 
    status_pompa2 = false; 
    status_microwave=false;;
    status_pengaduk2 = false;;
    batch2 = false;

    digitalWrite(relay_1_microwave, HIGH);
    digitalWrite(relay_2_motor1, HIGH);
    digitalWrite(relay_3_motor2, HIGH);
    digitalWrite(relay_pump_1, HIGH);
    digitalWrite(relay_pump_2, HIGH);

  }
}

BLYNK_WRITE(V0)
{
  int state = param.asInt();

  if(state){
    starter = true; 
    
    if(startMs!=0){
      buzzer();
    }
  }else{
    starter = false; 
  }
}

BLYNK_WRITE(V5)
{
    TimeInputParam t(param);
    if(t.hasStartTime())
    {
        startMs =
          t.getStartHour()   * 3600000UL +
          t.getStartMinute() * 60000UL +
          t.getStartSecond() * 1000UL;
        
        Serial.print("Start Time : ");
        Serial.println(startMs);

    }

    if(t.hasStopTime())
    {
      t.getStopHour();
      t.getStopMinute();
    }
}


