#include "esp_camera.h"
#include <WiFi.h>

//  Tambahkan definisi model kamera
#define CAMERA_MODEL_AI_THINKER 

#include "camera_pins.h"

// Konfigurasi Wi-Fi
#define ssid  "Tinkpad"         // Ganti dengan nama Wi-Fi Anda
#define password  "12345678" // Ganti dengan password Wi-Fi Anda

#define LED_GPIO 4 // Pin untuk mengontrol LED


void startCameraServer();

void setup() {
  Serial.begin(115200);
  pinMode(LED_GPIO, OUTPUT);
  digitalWrite(LED_GPIO, LOW);

  // Konfigurasi kamera
  camera_config_t config;
  config.ledc_channel = LEDC_CHANNEL_0;
  config.ledc_timer = LEDC_TIMER_0;
  config.pin_d0 = Y2_GPIO_NUM;
  config.pin_d1 = Y3_GPIO_NUM;
  config.pin_d2 = Y4_GPIO_NUM;
  config.pin_d3 = Y5_GPIO_NUM;
  config.pin_d4 = Y6_GPIO_NUM;
  config.pin_d5 = Y7_GPIO_NUM;
  config.pin_d6 = Y8_GPIO_NUM;
  config.pin_d7 = Y9_GPIO_NUM;
  config.pin_xclk = XCLK_GPIO_NUM;
  config.pin_pclk = PCLK_GPIO_NUM;
  config.pin_vsync = VSYNC_GPIO_NUM;
  config.pin_href = HREF_GPIO_NUM;
  config.pin_sscb_sda = SIOD_GPIO_NUM;
  config.pin_sscb_scl = SIOC_GPIO_NUM;
  config.pin_pwdn = PWDN_GPIO_NUM;
  config.pin_reset = RESET_GPIO_NUM;
  config.xclk_freq_hz = 20000000;
  config.pixel_format = PIXFORMAT_JPEG;

  if (psramFound()) {
    config.frame_size = FRAMESIZE_UXGA;
    config.jpeg_quality = 10;
    config.fb_count = 2;
  } else {
    config.frame_size = FRAMESIZE_SVGA;
    config.jpeg_quality = 12;
    config.fb_count = 1;
  }

  // Inisialisasi kamera
  esp_err_t err = esp_camera_init(&config);
  if (err != ESP_OK) {
    Serial.printf("Kamera inisialisasi gagal, error: 0x%x", err);
    return;
  }

  // Koneksi Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWi-Fi terhubung.");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());

  startCameraServer();
}

void loop() {
  // Ambil frame dari kamera
  camera_fb_t* fb = esp_camera_fb_get();
  if (!fb) {
    Serial.println("Gagal mengambil frame!");
    return;
  }

  // Deteksi wajah (pastikan library dan fungsi sudah benar)
  if (fb->format == PIXFORMAT_JPEG) {
    dl_matrix3du_t* image_matrix = dl_matrix3du_alloc(1, fb->width, fb->height, 3);
    if (image_matrix) {
      box_array_t* faces = face_detect(image_matrix, fb->width, fb->height);
      if (faces && faces->len > 0) {
        Serial.printf("Wajah terdeteksi: %d\n", faces->len);
        digitalWrite(LED_GPIO, HIGH); // Hidupkan LED
        delay(1000);
        digitalWrite(LED_GPIO, LOW);  // Matikan LED
      }
      dl_matrix3du_free(image_matrix);
    }
  }
  esp_camera_fb_return(fb);
}


void startCameraServer() {
  WiFiServer server(80);
  server.begin();
  Serial.println("Server dimulai.");

  while (true) {
    WiFiClient client = server.available();
    if (client) {
      Serial.println("Klien terhubung.");
      String request = client.readStringUntil('\r');
      Serial.println(request);
      client.println("HTTP/1.1 200 OK");
      client.println("Content-Type: image/jpeg");
      client.println("Connection: close");
      client.println();

      camera_fb_t* fb = esp_camera_fb_get();
      if (!fb) {
        Serial.println("Gagal mengambil frame!");
        continue;
      }
      client.write(fb->buf, fb->len);
      esp_camera_fb_return(fb);
      client.stop();
      Serial.println("Klien terputus.");
    }
  }
}
