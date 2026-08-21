
#include <Arduino.h>
#if defined(ESP32)
  #include <WiFi.h>
#elif defined(ESP8266)
  #include <ESP8266WiFi.h>
#endif
#include <Firebase_ESP_Client.h>

#define LED 2

#include "addons/TokenHelper.h"
#include "addons/RTDBHelper.h"

// Insert your network credentials
#define WIFI_SSID "192.168.65.10"
#define WIFI_PASSWORD "88888888"
#define USER_EMAIL "jamilfebrian68@gmail.com"
#define USER_PASSWORD "12345678"
#define USER_UUID "3rkRbklEctY3TGaXLYZMZLHvT0z2"

// Insert Firebase project API Key
#define API_KEY "AIzaSyAT6fK0djMUblHDICdb6XpxkFDiGDkm4R4"
#define DATABASE_URL "https://login-page-77ed3-default-rtdb.asia-southeast1.firebasedatabase.app/" 

FirebaseData fbdo;
FirebaseAuth auth;
FirebaseConfig config;

unsigned long sendDataPrevMillis = 0;
bool state1 = false;
bool signupOK = false;

bool lampu1,lampu2;
int intValue;
float floatValue;

void setup(){
  pinMode(LED, OUTPUT);
  Serial.begin(115200);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to Wi-Fi");
  while (WiFi.status() != WL_CONNECTED){
    Serial.print(".");
    delay(300);
  }
  Serial.println();
  Serial.print("Connected with IP: ");
  Serial.println(WiFi.localIP());
  Serial.println();

  //setting konfigurasi login firebase
  config.api_key = API_KEY;
  config.database_url = DATABASE_URL;
  auth.user.email = USER_EMAIL;
  auth.user.password = USER_PASSWORD;

  /* Sign up */
  signupOK = true;

  config.token_status_callback = tokenStatusCallback;
  
  Firebase.begin(&config, &auth);
  Firebase.reconnectWiFi(true);
}

void loop(){



  
  
  if (Firebase.ready() && signupOK && (millis() - sendDataPrevMillis > 500 || sendDataPrevMillis == 0)) {
    sendDataPrevMillis = millis();
      if (Firebase.RTDB.getInt(&fbdo, "/kontrol/"USER_UUID"/lampu1")) {
          Serial.println("SUCCES");
            if(fbdo.intData() == 1){
              digitalWrite(LED,HIGH);
            }else{
              digitalWrite(LED,LOW);
            }
       }
        else {
          Serial.println(fbdo.errorReason());
        }
  }
//    if (Firebase.RTDB.setInt(&fbdo, "kontrol/lampu1", logic)){
//      Serial.println("PASSED");
//      Serial.println("PATH: " + fbdo.dataPath());
//      Serial.println("TYPE: " + fbdo.dataType());
//    }
//    else {
//      Serial.println("FAILED");
//      Serial.println("REASON: " + fbdo.errorReason());
//    } 
//
//    logic = !logic;
//
//    if (Firebase.RTDB.setInt(&fbdo, "kontrol/lampu2", logic)){
//      Serial.println("PASSED");
//      Serial.println("PATH: " + fbdo.dataPath());
//      Serial.println("TYPE: " + fbdo.dataType());
//    }
//    else {
//      Serial.println("FAILED");
//      Serial.println("REASON: " + fbdo.errorReason());
//    }
//  }
}
