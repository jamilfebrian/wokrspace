#include <WiFi.h>
#include "FirebaseESP32.h"

// Ganti dengan informasi WiFi Anda
#define WIFI_SSID "Tinkpad"
#define WIFI_PASSWORD "12345678"
#define USER_EMAIL "jamilfebrian68@gmail.com"
#define USER_PASSWORD "12345678"

// Ganti dengan informasi Firebase Anda
#define API_KEY "AIzaSyAT6fK0djMUblHDICdb6XpxkFDiGDkm4R4"
#define DATABASE_URL "https://login-page-77ed3-default-rtdb.asia-southeast1.firebasedatabase.app/" 


FirebaseAuth auth;
FirebaseConfig config;

FirebaseData firebaseData;

void setup() {
  Serial.begin(115200);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to WiFi");
  
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(300);
  }
  Serial.println();
  Serial.println("Connected to WiFi");

   config.api_key = API_KEY;
  config.database_url = DATABASE_URL;
  auth.user.email = USER_EMAIL;
  auth.user.password = USER_PASSWORD;

  /* Sign up */
//  signupOK = true;

  config.token_status_callback = tokenStatusCallback; //see addons/TokenHelper.h
  

  Firebase.begin(&config, &auth);
  Firebase.reconnectWiFi(true);

  // Ganti "/path/to/your/value" dengan path ke nilai yang ingin Anda ambil
  if (Firebase.getInt(firebaseData, "/kontrol/lampu1")) {
    if (firebaseData.dataType() == "boolean") {
      Serial.print("The value is: ");
      Serial.println(firebaseData.intData());
    }
  } else {
    Serial.print("Failed to read value: ");
    Serial.println(firebaseData.errorReason());
  }
}

void loop() {
  // Looping tasks jika diperlukan
}
