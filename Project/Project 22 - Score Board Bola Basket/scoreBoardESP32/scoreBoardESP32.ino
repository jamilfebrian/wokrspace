#include <esp_now.h>
#include <WiFi.h>

typedef struct struct_message {
  char a[32]; 
} struct_message;
struct_message myData;

void OnDataRecv(const uint8_t * mac, const uint8_t *incomingData, int len) {
  memcpy(&myData, incomingData, sizeof(myData));
  String dataKirim = String(myData.a);

    Serial2.println(dataKirim);
    Serial.println(dataKirim);
}

void setup() {
  Serial.begin(115200);
  Serial2.begin(9600);
  WiFi.mode(WIFI_STA);

  // Init ESP-NOW
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }
  
  esp_now_register_recv_cb(OnDataRecv);
}

void loop() {
  
}
