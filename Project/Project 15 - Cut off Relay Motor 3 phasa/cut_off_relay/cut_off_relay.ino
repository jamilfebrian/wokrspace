#include <DHT.h>
#include <SoftwareSerial.h>

#define dht11       4
#define sensorApi   5
#define sensorGas   6
#define analogGas   A0
#define cutoffRelay 8
#define powerRelay  9

SoftwareSerial myserial(2, 3);
DHT dht(dht11, DHT11);

String pesan, dataKirim;
float humi, suhu;
int gasValue;
bool flame, gas;
unsigned long cTime = 0;

void setup() {
  pinMode(flame, INPUT);
  pinMode(gas, INPUT); pinMode(analogGas, INPUT);
  pinMode(cutoffRelay, OUTPUT);
  pinMode(powerRelay, OUTPUT);
  digitalWrite(powerRelay, HIGH);

  Serial.begin(115200);
  myserial.begin(115200);
  dht.begin();
}

void loop() {
  humi = dht.readHumidity();
  suhu = dht.readTemperature();
  gas = digitalRead(sensorGas);
  flame = digitalRead(sensorApi);
  gasValue = analogRead(analogGas);

  dataKirim = String(suhu) + " " + String(humi) + " " + String(gasValue) + " 1" + String(flame);

  while (myserial.available()) {
    pesan += char(myserial.read());
  } pesan.trim();

  if (millis() - cTime >= 500) {
    Serial.println(dataKirim);
    myserial.println(dataKirim);
    if (pesan == "1") {
      digitalWrite(powerRelay, LOW);
    } else if (pesan == "0") {
      digitalWrite(powerRelay, HIGH);
    } else if (pesan == "off") {
      digitalWrite(cutoffRelay, HIGH);
    }
    cTime = millis();
    pesan = "";
  }
}
