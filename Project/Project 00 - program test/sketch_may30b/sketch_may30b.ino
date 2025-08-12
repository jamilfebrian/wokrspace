#include <SPI.h>
#include <LoRa.h>

#define ss 10
#define rst 9
#define dio0 2

void setup() {
  Serial.begin(9600);
  while (!Serial);

  LoRa.setPins(ss, rst, dio0); // NSS, RST, DIO0 (ubah sesuai wiring kamu)
  if (!LoRa.begin(433E6)) {
    Serial.println("LoRa init gagal");
    while (1);
  }

  LoRa.setSyncWord(0xF3); // harus sama dengan transmitter
  Serial.println("LoRa Receiver siap");
}

void loop() {
  int packetSize = LoRa.parsePacket();
  Serial.println("X");
  if (packetSize) {
    String received = "";
    while (LoRa.available()) {
      received += (char)LoRa.read();
    }

    Serial.print("Diterima: ");
    Serial.println(received);
  }
}
