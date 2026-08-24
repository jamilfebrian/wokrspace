
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
#define MotorRelay  2
#define ServoPin    27
#define SENSOR_PIN  4 

// Pin sensor suhu MAX6675
#define SO_PIN  19
#define CS_PIN  5
#define SCK_PIN 18

Servo myservo; 

static bool activated = false;
static unsigned long startMs = 0;
unsigned long timerMillis = 0;

// Speed Sensor Variabel
const int SLOTS_PER_REV = 1;
volatile unsigned long last_micros = 0;
volatile unsigned long pulse_interval = 0;
volatile bool new_pulse = false;
static float rpm;

int setPoint = 0;

LiquidCrystal_I2C lcd(0x27, 16, 2);
MAX6675 thermocouple(SCK_PIN, CS_PIN, SO_PIN);

void IRAM_ATTR optoISR() {
  unsigned long current_micros = micros();
  unsigned long duration = current_micros - last_micros;
  
  // Debounce optik sederhana (mengabaikan pembacaan di bawah 1ms)
  if (duration > 1000) { 
    pulse_interval = duration;
    last_micros = current_micros;
    new_pulse = true;
  }
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
  
  if (new_pulse) {
    // Ambil data interval secara aman dari ISR
    noInterrupts();
    unsigned long interval = pulse_interval;
    new_pulse = false;
    interrupts();

    // Rumus: (60.000.000 mikrodetik / interval per pulsa) / jumlah lubang
    rpm = (60000000.0 / interval) / SLOTS_PER_REV;
  }

  // Timeout jika roda berhenti (> 2 detik tanpa pulsa)
  if (micros() - last_micros > 2000000) {
    rpm = 0.0;
  }

  // Cetak hasil ke Serial Monitor tiap 500 ms
  static unsigned long last_print = 0;
  if (millis() - last_print >= 500) {
    last_print = millis();
    Serial.print("Kecepatan Rotasi: ");
    Serial.print(rpm, 1);
    Serial.println(" RPM");
  }
}

void setup() {

  Serial.begin(115200);
  pinMode(BuzzerPin, OUTPUT);
  pinMode(HaeterRelay, OUTPUT);
  pinMode(MotorRelay, OUTPUT);
  pinMode(SENSOR_PIN, INPUT_PULLUP);

  lcd.init(); lcd.backlight(); lcd.clear();

  connectWifi();

  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, password);
  Serial.println("terhubung");

  attachInterrupt(digitalPinToInterrupt(SENSOR_PIN), optoISR, RISING);

  ESP32PWM::allocateTimer(0);
  myservo.setPeriodHertz(50);
  myservo.attach(ServoPin, 500, 2400);

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
      digitalWrite(HaeterRelay, LOW);
    } else if(suhu < 205){
      digitalWrite(HaeterRelay, HIGH);
    }
 
  if(activated && startMs!=0){
    if(millis() - timerMillis < startMs){
      digitalWrite(MotorRelay, HIGH);
      myservo.write(setPoint);
    } else {
      Blynk.virtualWrite(V1, 0);
      Blynk.virtualWrite(V3, 0);
      myservo.write(0); setPoint=0;
      digitalWrite(MotorRelay, LOW);
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
    digitalWrite(MotorRelay, LOW);
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
