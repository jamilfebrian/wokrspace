const int sensorPin = 26;  // Pin sensor MPX5 terhubung ke pin 26 ESP32

void setup() {
  Serial.begin(115200);  // Inisialisasi Serial Monitor
}

void loop() {
  // Baca nilai analog dari sensor
  int sensorValue = analogRead(sensorPin);
  
  // Konversi nilai analog ke tekanan (dalam pascal)
  // Misalnya, sensor MPX5 memberikan output antara 0 hingga 5V, dan ini dikonversi ke nilai tekanan
  // Tekanan dalam Pascal dihitung menggunakan rumus:
  // tekanan (Pascal) = (sensorValue / 4095.0) * 5000000
  // Catatan: Ganti nilai '5000000' dengan konstanta yang sesuai untuk sensor Anda
  
  float tekanan = (sensorValue / 4095.0) * 5000000;  // Konversi nilai ADC ke tekanan dalam Pascal
  
  // Tampilkan nilai tekanan di Serial Monitor
  Serial.print("Tekanan: ");
  Serial.print(tekanan);
  Serial.println(" Pascal");
  
  delay(1000);  // Tunggu 1 detik sebelum membaca ulang
}
