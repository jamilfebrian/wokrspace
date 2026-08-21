
#define BLYNK_TEMPLATE_ID "TMPL6plP2ulU5"
#define BLYNK_TEMPLATE_NAME "Project Testing"
#define BLYNK_AUTH_TOKEN "eMnTBrDWICpLktidOXI4P-r5VqlyM0um"

#include <WiFi.h>
#include <max6675.h>
#include <BlynkSimpleEsp32.h>
#include <ESP32Servo.h>
#include <LiquidCrystal_I2C.h> 

char ssid[] = "Tinkpad";
char password[] = "12345678";

#define BuzzerPin   23
#define HaeterRelay 12
#define ServoPin    13
#define HALL_PIN 15

// Pin sensor suhu MAX6675
#define SO_PIN  19
#define CS_PIN  5
#define SCK_PIN 18

Servo myservo; 

static bool activated = false;
static unsigned long startMs = 0;
unsigned long timerMillis = 0;

const int PULSES_PER_REV = 4;
const unsigned long TIMEOUT = 2000000;
volatile unsigned long lastPulseTime = 0;
volatile unsigned long pulseInterval = 0;
float rpm;

int setPoint = 0;

LiquidCrystal_I2C lcd(0x27, 16, 2);
MAX6675 thermocouple(SCK_PIN, CS_PIN, SO_PIN);

void IRAM_ATTR hallInterrupt(){
  unsigned long currentTime = micros();
  if (lastPulseTime != 0)  {
    pulseInterval = currentTime - lastPulseTime;
  }
  lastPulseTime = currentTime;
}

void buzzer(int index = 3, int timerBuzzer = 80){
   for(int i=0; i<index; i++){
      digitalWrite(BuzzerPin, HIGH);
      delay(timerBuzzer);
      digitalWrite(BuzzerPin, LOW);
      delay(timerBuzzer);
  }
}

void connectWifi(){
  buzzer(1);
  Serial.println("Connecting to WiFi...");
      lcd.setCursor(0,0); lcd.print("Menghubungkan Ke");
      lcd.setCursor(0,1); lcd.print("Jaringan Wifi");
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000); 
    Serial.print("."); lcd.print(".");
  }  Serial.println("Wifi Terhubung");
       lcd.setCursor(0,0); lcd.print(" Jaringan Wifi  ");
       lcd.setCursor(0,1); lcd.print("   Terhubung    ");
       buzzer(2);
}

void HallSensor(){
  unsigned long interval;
  unsigned long lastPulse;

  noInterrupts();
  interval = pulseInterval;
  lastPulse = lastPulseTime;
  interrupts();

  unsigned long currentTime = micros();
  rpm = 0;

  if (lastPulse != 0 && (currentTime - lastPulse) < TIMEOUT && interval > 0){
    rpm = (60000000.0 / interval) / PULSES_PER_REV;
  } else {
    rpm = 0;
  }
}

void setup() {

  Serial.begin(115200);
  pinMode(BuzzerPin, OUTPUT);
  pinMode(HaeterRelay, OUTPUT);
  pinMode(HALL_PIN, INPUT_PULLUP);

  lcd.init(); lcd.backlight(); lcd.clear();

  connectWifi();

  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, password);
  Serial.println("terhubung");

  attachInterrupt(
    digitalPinToInterrupt(HALL_PIN),
    hallInterrupt,
    FALLING
  );
  
  myservo.attach(ServoPin);

  Blynk.virtualWrite(V1, 0);
  Blynk.virtualWrite(V3, 0);

  int pos = 0; 
  myservo.write(pos);
		delay(15);
}

void loop() {
  Blynk.run();

  HallSensor();
  double suhu = thermocouple.readCelsius();
  Blynk.virtualWrite(V0, suhu);
  Blynk.virtualWrite(V2, rpm);

  lcd.setCursor(0,0); lcd.print(" Suhu : "); lcd.print(suhu); lcd.print("   ");
  lcd.setCursor(0,1); lcd.print(" RPM  : "); lcd.print(rpm); lcd.print("   ");
  delay(100);

  
    if(suhu >= 210){
      digitalWrite(HaeterRelay, HIGH);
    } else if(suhu < 205){
      digitalWrite(HaeterRelay, LOW);
    }
 
  if(activated && startMs!=0){
    if(millis() - timerMillis < startMs){
      myservo.write(setPoint);
    } else {
      Blynk.virtualWrite(V1, 0);
      Blynk.virtualWrite(V3, 0);
      myservo.write(0); setPoint=0;
      activated = false;
    }
  } else {
    timerMillis = millis();
  }
}

BLYNK_WRITE(V1){
  int statusRelay = param.asInt();

  if (statusRelay == 1) {
    activated = true;
    if(startMs!=0){
      buzzer();
    }
  } else {
    activated = false;
    setPoint = 0;
    Blynk.virtualWrite(V3, 0);
    myservo.write(setPoint);
  }
}

BLYNK_WRITE(V3){
  int levelSpeed = param.asInt();

  if (levelSpeed == 1) {
    setPoint = 60;
  } else if (levelSpeed == 2) {
    setPoint = 120;
  } else if (levelSpeed == 3) {
    setPoint = 180;
  } else {
    setPoint = 0;
  }
}

BLYNK_WRITE(V5){
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

    if(activated){
      buzzer();
    }
}
