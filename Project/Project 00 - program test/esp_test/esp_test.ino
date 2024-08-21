#include <Wire.h>
#include <Adafruit_MLX90614.h>
#include "MAX30100_PulseOximeter.h"

// Inisialisasi objek untuk sensor suhu MLX90614
Adafruit_MLX90614 mlx = Adafruit_MLX90614();

// Inisialisasi objek untuk sensor detak jantung MAX30100
PulseOximeter pox;

uint32_t tsLastReport = 0;

void setup() {
  Serial.begin(115200);
  
  // Inisialisasi sensor MLX90614
  mlx.begin();

  // Inisialisasi sensor MAX30100
  pox.begin();

  // Atur callback ketika detak jantung baru terdeteksi
  pox.setOnBeatDetectedCallback(onBeatDetected);
}

void loop() {
  // Proses data dari sensor MAX30100
  pox.update();

  // Baca suhu dari sensor MLX90614
  float suhuObjek = mlx.readObjectTempC();
  float suhuAmbien = mlx.readAmbientTempC();

  // Setiap 1 detik, tampilkan data suhu, SpO2, dan detak jantung
  if (millis() - tsLastReport > 1000) {
    tsLastReport = millis();

    Serial.print("Suhu Objek: ");
    Serial.print(suhuObjek);
    Serial.println(" °C");

    Serial.print("Suhu Ambien: ");
    Serial.print(suhuAmbien);
    Serial.println(" °C");

    Serial.print("Detak Jantung: ");
    Serial.print(pox.getHeartRate());
    Serial.print(" bpm / SpO2: ");
    Serial.print(pox.getSpO2());
    Serial.println(" %");
  }
}

// Fungsi callback saat detak jantung terdeteksi
void onBeatDetected() {
  Serial.println("Detak jantung terdeteksi!");
}
