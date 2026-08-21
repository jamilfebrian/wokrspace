#include <ESP8266WiFi.h>
#include <PubSubClient.h>

const char* ssid         = "Kost Bukida";
const char* password     = "30072026";
const char* mqtt_server  = "broker.hivemq.com";
const int   mqtt_port    = 1883;
const char* topic_relay  = "jamil/iot/relay1";

#define RELAY_PIN D5

WiFiClient espClient;
PubSubClient client(espClient);

// Fungsi Callback: Dieksekusi SECARA INSTAN ketika ada pesan masuk
void callback(char* topic, byte* payload, unsigned int length) {
  String message = "";
  for (int i = 0; i < length; i++) {
    message += (char)payload[i];
  }

  Serial.print("Pesan diterima di topik [");
  Serial.print(topic);
  Serial.print("]: ");
  Serial.println(message);

  // Kontrol Relay berdasarkan isi pesan
  if (message == "1") {
    digitalWrite(RELAY_PIN, LOW);  // Relay ON (Active Low)
    Serial.println("Relay dinyalakan!");
  } else if (message == "0") {
    digitalWrite(RELAY_PIN, HIGH); // Relay OFF (Active Low)
    Serial.println("Relay dimatikan!");
  }
}

void reconnect() {
  while (!client.connected()) {
    Serial.print("Menghubungkan ke MQTT Broker...");
    String clientId = "ESP8266Client-";
    clientId += String(random(0xffff), HEX);

    if (client.connect(clientId.c_str())) {
      Serial.println("Terhubung!");
      // Subscribe ke topik relay
      client.subscribe(topic_relay);
    } else {
      Serial.print("Gagal, rc=");
      Serial.print(client.state());
      Serial.println(" Coba lagi dalam 5 detik");
      delay(5000);
    }
  }
}

void setup() {
  Serial.begin(115200);
  pinMode(RELAY_PIN, OUTPUT);
  digitalWrite(RELAY_PIN, HIGH); // Default OFF

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi Terhubung!");

  client.setServer(mqtt_server, mqtt_port);
  client.setCallback(callback); // Set fungsi penerima pesan
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop(); // Penting: Menjaga koneksi tetap hidup dan mengecek pesan baru
}