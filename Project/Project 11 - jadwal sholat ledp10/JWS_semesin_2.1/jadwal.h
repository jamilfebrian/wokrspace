#ifndef JADWAL_H
#define JADWAL_H

#include <EEPROM.h>
#include <RTC_Semesin.h>

#include "setting.h"

struct JamDanMenitAktif {
  bool aktif;
  uint16_t waktu;
};

struct JamDanMenitJadwal {
  bool aktif;
  uint16_t waktu;
  int8_t suaraTartil;
  int8_t suaraTarhim;
  uint8_t index;
  uint8_t jumlahPengingat;
  uint8_t jumlahAlarm;
};


RTC_DS3231 rtc;
DateTime now;

JamDanMenitJadwal jadwalBerikutnya;
JamDanMenitAktif weekerBerikutnya;
uint16_t offsetJadwalEEPROM;

uint16_t hitungMundurTartil;
uint16_t hitungMundurTarhim;
uint16_t hitungMundurPengingat;
uint8_t hitungMundurAlarm;
uint16_t hitungMundurAlarmMasukWaktuShalat;
uint16_t hitungMundurAzan;//terhitung sejak waktu masuk
uint16_t hitungMundurIqamah;//terhitung sejak selesai azan
uint8_t hitungMundurAlarmIqamah;//kali

bool pesanSetelahAzan;
bool pesanSebelumShalat;
uint16_t hitungMundurPeringatanSimbol;
uint16_t hitungMundurShalat;

bool initHitungMundurTartil;
bool initHitungMundurTarhim;
bool initHitungMundurPengingat;
bool initHitungMundurAlarmMasukWaktuShalat;
bool initHitungMundurAzan;
bool initHitungMundurIqamah;
bool initHitungMundurPeringatanSimbol;
bool initHitungMundurShalat;

void cekWeeker()
{
  uint16_t unixWaktu  = (now.jam * 60) + now.menit;
  weekerBerikutnya.aktif = false;

  for (byte i = 0; i < 5; i++)
  {
    if (EEPROM.read(offsetof(Setting, weekerAktif) + i))
    {
      byte weekerJam = EEPROM.read(offsetof(Setting, weeker) + (i * 2) + 0);
      byte weekerMenit = EEPROM.read(offsetof(Setting, weeker) + (i * 2) + 1);
      uint32_t weekerAlarm = (weekerJam * 60) + weekerMenit;

      if (weekerAlarm > unixWaktu)
      {
        weekerBerikutnya.aktif = true;
//        weekerBerikutnya.jam = weekerJam;
//        weekerBerikutnya.menit = weekerMenit;
        weekerBerikutnya.waktu = weekerAlarm;

        break;
      }
    }
  }

#if serialDebug > 1
  Serial.print(F("weekerBerikutnya\r\njam="));
  Serial.println(weekerBerikutnya.waktu / 60);
  Serial.print(F("menit="));
  Serial.println(weekerBerikutnya.waktu % 60);
#endif

}
void cekJadwal()
{
  uint16_t unixWaktu  = (now.jam * 60) + now.menit;
  jadwalBerikutnya.aktif = false;

  for (int i = 0; i < jumlahWaktuShalat; i++)
  {
    int8_t indexShalatWajib = konversiShalatWajib[i];
    uint16_t jadwalJam = EEPROM.read(offsetJadwalEEPROM + (i * 2) + 0);
    uint16_t jadwalMenit = EEPROM.read(offsetJadwalEEPROM + (i * 2) + 1);
    byte lamaPengingat = EEPROM.read(offsetof(Setting, jarakAlarm) + i);
    
    if (indexShalatWajib != -1)
    {
      byte lamaTartil = 0;
      byte lamaTarhim = 0;
      if (EEPROM.read(offsetof(Setting, aktifTartil) + indexShalatWajib))
      {
        lamaTartil = EEPROM.read(offsetof(Setting, lamaTartil) + indexShalatWajib);
      }
      if (EEPROM.read(offsetof(Setting, aktifTarhim) + indexShalatWajib))
      {
        lamaTarhim = EEPROM.read(offsetof(Setting, lamaTarhim) + indexShalatWajib);
      }

      uint32_t unixJadwal = ((jadwalJam * 60) + jadwalMenit) - (lamaTarhim + lamaTartil + lamaPengingat);

      if (unixJadwal > unixWaktu)
      {
        jadwalBerikutnya.aktif = true;
        jadwalBerikutnya.waktu = unixJadwal;
        jadwalBerikutnya.jumlahPengingat = lamaPengingat;

        if (lamaTartil)
        {
          jadwalBerikutnya.suaraTartil = EEPROM.read(offsetof(Setting, suaraTartil) + indexShalatWajib);
        }
        else
        {
          jadwalBerikutnya.suaraTartil = -1;
        }
        if (lamaTarhim)
        {
          jadwalBerikutnya.suaraTarhim = EEPROM.read(offsetof(Setting, suaraTarhim) + indexShalatWajib);
        }
        else
        {
          jadwalBerikutnya.suaraTarhim = -1;
        }

        jadwalBerikutnya.index = i;
        hitungMundurTartil = lamaTartil * 60;
        initHitungMundurTartil = true;
        hitungMundurTarhim = lamaTarhim * 60;
        initHitungMundurTarhim = true;

        hitungMundurAlarmMasukWaktuShalat = EEPROM.read(offsetof(Setting, aktifAlarmWaktuMasuk)) * 5;
        initHitungMundurAlarmMasukWaktuShalat = true;

        hitungMundurPengingat = lamaPengingat * 60;
        if(hitungMundurPengingat)
        {
          initHitungMundurPengingat = true;
          hitungMundurAlarm = EEPROM.read(offsetof(Setting, jumlahAlarm));
        }


        if ((now.hari - 1 == Jumat) && (i == Zuhur)) //Hari jumat
        {
          hitungMundurAzan = 0;
          hitungMundurIqamah = 0;

          pesanSetelahAzan = false;
          hitungMundurAlarmIqamah = 0;

          pesanSebelumShalat = false;
          hitungMundurPeringatanSimbol = 0;
          hitungMundurShalat = EEPROM.read(offsetof(Setting, lamaShalatJumat)) * 60;//detik
          initHitungMundurShalat = true;
        }
        else
        {
          hitungMundurAzan = EEPROM.read(offsetof(Setting, lamaAzan)) * 60; //menit
          if (hitungMundurAzan)
          {
            initHitungMundurAzan = true;


            hitungMundurIqamah = EEPROM.read(offsetof(Setting, lamaIqamah) + indexShalatWajib) * 60; //menit
            initHitungMundurIqamah = true;
            if (EEPROM.read(offsetof(Setting, aktifAlarmIqamah) + indexShalatWajib))
            {
              hitungMundurAlarmIqamah = EEPROM.read(offsetof(Setting, jumlahAlarmIqamah));
            }
            else
              hitungMundurAlarmIqamah = 0;

            if (!hitungMundurIqamah)
            {
              hitungMundurIqamah = hitungMundurAlarmIqamah;
            }

            EEPROM.get(offsetof(Setting, aktifPesanSetelahAzan), pesanSetelahAzan);

            EEPROM.get(offsetof(Setting, aktifPesanSebelumShalat), pesanSebelumShalat);

            if (EEPROM.read(offsetof(Setting, simbolKakiBersentuhan)) || EEPROM.read(offsetof(Setting, simbolHP)) || EEPROM.read(offsetof(Setting, simbolDilarangBerisik)))
            {
              EEPROM.get(offsetof(Setting, lamaSimbolGambar), hitungMundurPeringatanSimbol);//detik
              initHitungMundurPeringatanSimbol = true;
            }
          }

          hitungMundurShalat = EEPROM.read(offsetof(Setting, lamaShalat) + indexShalatWajib) * 60;
          initHitungMundurShalat = true;
        }

        break;
      }
    }
    else if (EEPROM.read(offsetof(Setting, aktifAlarm) + i))
    {
      uint32_t unixJadwal = ((jadwalJam * 60) + jadwalMenit) - lamaPengingat;

      if (unixJadwal > unixWaktu)
      {
        jadwalBerikutnya.aktif = true;
        jadwalBerikutnya.waktu = unixJadwal;
        jadwalBerikutnya.index = i;

        hitungMundurPengingat = lamaPengingat * 60;
        if(hitungMundurPengingat)
        {
          initHitungMundurPengingat = true;
          hitungMundurAlarm = EEPROM.read(offsetof(Setting, jumlahAlarm));
        }
        
      }
    }
  }
#if serialDebug > 1
        Serial.print(F("jadwalBerikutnya\r\njam="));
        Serial.println(jadwalBerikutnya.waktu / 60);
        Serial.print(F("menit="));
        Serial.println(jadwalBerikutnya.waktu % 60);
        Serial.print(F("hitungMundurTarhim="));
        Serial.println(hitungMundurTarhim);
        Serial.print(F("hitungMundurAzan="));
        Serial.println(hitungMundurAzan);
        Serial.print(F("hitungMundurIqamah="));
        Serial.println(hitungMundurIqamah);
        Serial.print(F("hitungMundurAlarmIqamah="));
        Serial.println(hitungMundurAlarmIqamah);
        Serial.print(F("hitungMundurShalat="));
        Serial.println(hitungMundurShalat);
#endif
}

void setModeDemo()
{
  hitungMundurTartil = 10;//detik
  jadwalBerikutnya.suaraTartil = EEPROM.read(offsetof(Setting, suaraTartil) + jadwalBerikutnya.index);

  hitungMundurTarhim = 10;//detik
  jadwalBerikutnya.suaraTarhim = EEPROM.read(offsetof(Setting, suaraTarhim) + jadwalBerikutnya.index);

  uint32_t unixJadwal = ((uint32_t)now.jam * 60) + now.menit + 1;

  jadwalBerikutnya.aktif = true;
//  jadwalBerikutnya.jam = unixJadwal / 60;
//  jadwalBerikutnya.menit = unixJadwal % 60;
  jadwalBerikutnya.waktu = unixJadwal;
  jadwalBerikutnya.jumlahPengingat = 60;
  jadwalBerikutnya.jumlahAlarm = EEPROM.read(offsetof(Setting, jumlahAlarm));


  hitungMundurAzan = 10;
  //  initHitungMundurAzan = true;
  //  EEPROM.get(offsetof(Setting, aktifPesanSetelahAzan), pesanSetelahAzan);
  hitungMundurIqamah = 30;
  //  initHitungMundurIqamah = true;
  //  hitungMundurAlarmIqamah = EEPROM.read(offsetof(Setting, jumlahAlarm));
  //  EEPROM.get(offsetof(Setting, aktifPesanSebelumShalat), pesanSebelumShalat);
  //  EEPROM.get(offsetof(Setting, lamaSimbolGambar), hitungMundurPeringatanSimbol);//detik
  //  initHitungMundurPeringatanSimbol = true;
  hitungMundurShalat = 10;
  //  initHitungMundurShalat = true;

#if serialDebug > 1
  Serial.print(F("jadwalBerikutnya\r\njam="));
  Serial.println(jadwalBerikutnya.jam);
  Serial.print(F("menit="));
  Serial.println(jadwalBerikutnya.menit);
#endif
}

void updateJadwal()
{
  now = rtc.now();

  Tanggal tanggal = {now.tanggal, now.bulan, now.tahun};
  byte jadwalJam;
  byte jadwalMenit;
  uint16_t unixImsak;

  switch (EEPROM.read(offsetof(Setting, modeJadwal)))
  {
    case 0:
      ambilWaktuShalat(tanggal);
      offsetJadwalEEPROM = offsetof(Setting, dataPengaturan) + offsetof(DataPengaturan, jadwalMatahari);
      break;
    case 1:
      offsetJadwalEEPROM = offsetof(Setting, jadwalManual);
      break;
    case 2:
      offsetJadwalEEPROM = offsetof(Setting, jadwalOnline);
      jadwalJam = EEPROM.read(offsetJadwalEEPROM + (1 * 2) + 0);//subuh
      jadwalMenit = EEPROM.read(offsetJadwalEEPROM + (1 * 2) + 1);

      unixImsak = ((jadwalJam * 60) + jadwalMenit) - EEPROM.read(offsetPengaturanJadwal + offsetof(DataPengaturan, aturImsak));
      EEPROM.write(offsetJadwalEEPROM + (0 * 2) + 0, unixImsak / 60);//imsak
      EEPROM.write(offsetJadwalEEPROM + (0 * 2) + 1, unixImsak % 60);//imsak


      break;
  }

  cekJadwal();
//  cekAlarm();
  cekWeeker();
#if serialDebug > 1
  for (int i = 0; i < jumlahWaktuShalat; i++)
  {
    Serial.print(EEPROM.read(offsetJadwalEEPROM + (i * 2) + 0));
    Serial.print(F(":"));
    Serial.println(EEPROM.read(offsetJadwalEEPROM + (i * 2) + 1));
  }
#endif
}

#endif
