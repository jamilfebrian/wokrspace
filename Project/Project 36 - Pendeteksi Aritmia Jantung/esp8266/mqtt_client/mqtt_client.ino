#include <WiFi.h>
#include <PubSubClient.h>
 
#define WIFISSID "Tinkpad"     
#define PASSWORD "12345678"    
#define TOKEN "BBUS-NeisqfaJ6GNSCd8nm7UNIN07gnV0oT" 
#define MQTT_CLIENT_NAME "esp8266" 

#define VARIABLE_LABEL "myecg" 
#define DEVICE_LABEL "esp8266"
 
char mqttBroker[]  = "industrial.api.ubidots.com";
char payload[100];
char topic[150];
char str_sensor[10];

String dataArduino;

#define RXD2 16  // RX2 ESP32 (terhubung ke TX Arduino)
#define TXD2 17  // TX2 ESP32 (terhubung ke RX Arduino)
 
WiFiClient ubidots;
PubSubClient client(ubidots);
 
void callback(char* topic, byte* payload, unsigned int length) {
  char p[length + 1];
  memcpy(p, payload, length);
  p[length] = NULL;
  Serial.write(payload, length);
  Serial.println(topic);
}
 
void reconnect() {
  while (!client.connected()) {
    Serial.println("Attempting MQTT connection...");
    
    if (client.connect(MQTT_CLIENT_NAME, TOKEN, "")) {
      Serial.println("Connected");
    } else {
      Serial.print("Failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 2 seconds");
      delay(2000);
    }
  }
}

void setup() {
  Serial.begin(115200);
  Serial2.begin(9600, SERIAL_8N1, RXD2, TXD2);
  WiFi.begin(WIFISSID, PASSWORD);
 
  Serial.println();
  Serial.print("Waiting for WiFi...");
  
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  
  Serial.println("");
  Serial.println("WiFi Connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  client.setServer(mqttBroker, 1883);
  client.setCallback(callback);  
}
 
void loop() {
  if (!client.connected()) {
    reconnect();
  }

  if (Serial2.available()) {
        dataArduino = Serial2.readStringUntil('\n'); // Baca data dari Arduino
        Serial.print("Dari Arduino: ");
        Serial.println(dataArduino);
    }
 
  sprintf(topic, "%s%s", "/v1.6/devices/", DEVICE_LABEL);
  sprintf(payload, "%s", "");
  sprintf(payload, "{\"%s\":", VARIABLE_LABEL);
  
//  dtostrf(dataArduino, 4, 2, str_sensor);
  
  sprintf(payload, "%s {\"value\": %s}}", payload, dataArduino);
  Serial.println("Publishing data to Ubidots Cloud");
  Serial.print("topic : "); Serial.println(topic);
  Serial.print("payload : "); Serial.println(payload);
  client.publish(topic, payload);
  client.loop();
  delay(10);
}
