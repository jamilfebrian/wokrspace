#include <WiFi.h>
#include <ThingSpeak.h>

HardwareSerial SerialPort(2);

const char* ssid = "Tinkpad";
const char* password = "12345678";

WiFiClient client;

unsigned long channelID = 3413043;
const char *writeAPIKey = "RX4HBCP1HMJVV6ZT";

void setup() {
  Serial.begin(115200);
  SerialPort.begin(9600, SERIAL_8N1, 16, 17);

  Serial.println("ESP32 siap menerima data");

  WiFi.begin(ssid, password);
  Serial.print("Menghubungkan WiFi");

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("\nWiFi Terhubung");
  ThingSpeak.begin(client);
}

void loop() {

  if (SerialPort.available()) {

    String data = SerialPort.readStringUntil('\n');

    Serial.print("Data diterima: ");
    Serial.println(data);

    int idx1 = data.indexOf('|');
    int idx2 = data.indexOf('|', idx1 + 1);

    float setPoint = data.substring(0, idx1).toFloat();
    float RPM = data.substring(idx1 + 1, idx2).toFloat();
    float PWM = data.substring(idx2 + 1).toFloat();

    ThingSpeak.setField(1, setPoint);
    ThingSpeak.setField(2, RPM);
    ThingSpeak.setField(3, PWM);

    int statusCode = ThingSpeak.writeFields(channelID, writeAPIKey);
    if (statusCode == 200) {
      Serial.println("Data berhasil dikirim ke ThingSpeak");
    }
    else {
      Serial.print("Gagal mengirim data. Kode error: ");
      Serial.println(statusCode);
    }

    // Serial.println(setPoint);
    // Serial.println(RPM);
    // Serial.println(PWM);

    delay(15000);

  }

}