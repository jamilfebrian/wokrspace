#include <ESP8266WiFi.h>
#include <ThingESP.h>

// Konfigurasi WiFi
const char* wifiSsid = "Tinkpad";
const char* wifiPassword = "987654321";

// Konfigurasi Twilio
const char* twilioAccountSid = "ACc70b6cf34f9628bc482da7647a708bc8";
const char* twilioAuthToken = "ba68d04fa16ac51bb643d3e92eae91f";
const char* twilioPhoneNumber = "+6285268187811";
const char* twilioSandbox = "Bot deteksi";

ThingESP8266 thing("raraa", "botnodemcu", "botnodemcu1234");

int flameSensorPin = A0;
int ledPin = 2;

void setup() {
  Serial.begin(115200);
  pinMode(ledPin, OUTPUT);
  thing.SetWiFi(wifiSsid, wifiPassword);
  thing.initDevice();
}

String HandleResponse(String query) {
  if (query == "led on") {
    digitalWrite(ledPin, LOW);
    return "Done: LED Turned ON";
  } else if (query == "led off") {
    digitalWrite(ledPin, HIGH);
    return "Done: LED Turned OFF";
  } else if (query == "led status") {
    return digitalRead(ledPin) ? "LED is OFF" : "LED is ON";
  } else if (query == "fire status") {
    if (isFireDetected()) {
      sendWhatsAppMessage("Deteksi kebakaran! Segera periksa situasinya.");
      return "Deteksi kebakaran! Pemberitahuan telah dikirim.";
    } else {
      return "Tidak ada deteksi kebakaran.";
    }
  } else {
    return "Query tidak valid.";
  }
}

void loop() {
  thing.Handle();
}

bool isFireDetected() {
  int flameValue = analogRead(flameSensorPin);
  return flameValue > 500; // Sesuaikan dengan ambang batas deteksi api
}

String sendWhatsAppMessage(String message) {
  return message;
  // ... (lihat contoh sebelumnya untuk implementasi fungsi ini)
}
