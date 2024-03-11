#include <Wire.h>
#include "RTClib.h"

RTC_DS1307 rtc;

void setup() {
  Serial.begin(9600);
  Wire.begin();
  rtc.begin();

  // Set waktu awal RTC
  rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
}

void loop() {
  DateTime now = rtc.now();
  DateTime future = now + TimeSpan(0, 3, 0, 0) + TimeSpan(0, 0, 15, 0); // Tambahkan 3 jam ke waktu sekarang

  while (now < future) {
    Serial.print("Sisa waktu: ");
    Serial.print((future - now).hours());
    Serial.print(':');
    Serial.print((future - now).minutes());
    Serial.print(':');
    Serial.println((future - now).seconds());

    now = rtc.now();
    delay(100);
  }

  Serial.println("Waktu habis!");
}
