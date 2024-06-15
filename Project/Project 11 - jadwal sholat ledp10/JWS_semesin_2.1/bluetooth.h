#ifndef BLUETOOTH_H
#define BLUETOOTH_H

#include <BluetoothApk.h>
#include "setting.h"
#include "tampilan.h"
#include "jadwal.h"

BluetoothApk bluetoothApk(&Serial, "JWS2 SEMESIN.COM");
long millisAksesApk;
bool aksesApk;

char buffer[72];
uint16_t counterAlamat = 0;
uint16_t counterAlamatSebelumnya = 0;
uint16_t parameterSebelumnya = 0;


void cekBluetooth()
{
#if perlindunganPassword
  if (aksesApk)
  {
    if ((millisAksesApk < millis()) && (modeOperasi != modeShalat))
    {
      aksesApk = false;
    }
  }
#endif

  if (Serial.available())
  {
    byte i;
    char16 *ptr16 = (char16*)(buffer + 0);
    char8 *ptr8 = (char8*)(buffer + 16);
    char32 *ptr32 = (char32*)buffer;
    DateTime* ptrDateTime = (DateTime*)buffer;

    dmd.end();

    bluetoothApk.cekTransmisi();

    switch (bluetoothApk.data.perintah)
    {
      case inisialisasi:
        if (bluetoothApk.ambilBuffer(0, 0))
        {
          bluetoothApk.responData(&bluetoothApk.responOK, 1);
        }
        break;

#if perlindunganPassword
      case aksesMasuk:
        if (bluetoothApk.ambilBuffer(buffer + 24, sizeof(buffer) - 24))
        {
          EEPROM.get(offsetof(Setting, idPengguna), ptr16->teks);
          EEPROM.get(offsetof(Setting, kataSandi), ptr8->teks);
          if (
            (strncmp(buffer + 24,      buffer + 0, 16) == 0) &&
            (strncmp(buffer + 24 + 16, buffer + 16, 8) == 0)
          )
          {
            aksesApk = true;
            bluetoothApk.responData(&bluetoothApk.responOK, 1);
          }
          else
          {
            bluetoothApk.responData(&bluetoothApk.responDiscard, 1);
          }
        }
        else
        {
          bluetoothApk.responData(&bluetoothApk.responNull, 1);
        }
        break;
#endif
      case cekAkses:
#if perlindunganPassword
        bluetoothApk.responData(aksesApk ? &bluetoothApk.responOK : &bluetoothApk.responNull, 1);
#else
        bluetoothApk.responData(&bluetoothApk.responOK, 1);
#endif
        break;
    }

#if perlindunganPassword
    if (aksesApk)
#endif
    {
      millisAksesApk = millis() + lamaAksesApk;

      switch (bluetoothApk.data.perintah)
      {
        case aksesKeluar:
          if (bluetoothApk.ambilBuffer(0, 0))
          {
            bluetoothApk.responData(&bluetoothApk.responOK, 1);
            aksesApk = false;
          }
          break;
#if perlindunganPassword
        case gantiKataKunci:
          if (bluetoothApk.ambilBuffer(buffer + 24, sizeof(buffer) - 24))
          {
            EEPROM.get(offsetof(Setting, idPengguna), ptr16->teks);
            EEPROM.get(offsetof(Setting, kataSandi), ptr8->teks);

            if (
              (strncmp(buffer + 24,          buffer + 0, 16) == 0) &&
              (strncmp(buffer + 24 + 16,     buffer + 0  + 16, 8) == 0) &&
              (strncmp(buffer + 24 + 16 + 8, buffer + 24 + 16 + 8 + 8, 8) == 0)
            )
            {
              bluetoothApk.responData(&bluetoothApk.responOK, 1);
              for (i = 0; i < 8; i++)
              {
                EEPROM.update(offsetof(Setting, kataSandi) + i, *(buffer + 24 + 16 + 8 + i));
              }
            }
            else
            {
              bluetoothApk.responData(&bluetoothApk.responDiscard, 1);
            }
          }
          else
          {
            bluetoothApk.responData(&bluetoothApk.responNull, 1);
          }
          break;
#endif
        case simpanDataString:
          if (bluetoothApk.data.parameter == 1)
          {
            counterAlamat = 0;
          }
          if (parameterSebelumnya == bluetoothApk.data.parameter)
          {
            counterAlamat = counterAlamatSebelumnya;
          }
          counterAlamatSebelumnya = counterAlamat;
          parameterSebelumnya = bluetoothApk.data.parameter;
          
          if (bluetoothApk.ambilDataI2CEEPROM(counterAlamat, 8096))
          {
            bluetoothApk.responData(&bluetoothApk.responOK, 1);

            EEPROM.put(offsetof(Setting, alamatPesanTartil) + ((bluetoothApk.data.parameter - 1) * sizeof(uint16_t)), counterAlamat);
            if (bluetoothApk.data.parameter == 16)
            {
              counterAlamat = 0;
              counterAlamatSebelumnya = 0;
            }
            else
            {
              counterAlamat += bluetoothApk.data.panjang;
              if (counterAlamat & 0x0F)
              {
                counterAlamat &= 0xFFF0;
                counterAlamat += 16;
              }
            }
          }
          else
          {
            bluetoothApk.responData(&bluetoothApk.responNull, 1);
          }
          break;
        case masukModeEntri:
          break;
        case keluarModeEntri:
          break;
        case ambilData:
          break;
        case simpanData:
          switch (bluetoothApk.data.parameter)
          {
            case simpanPengaturanJadwal:
              if (bluetoothApk.ambilDataEEPROM(offsetof(Setting, jadwalManual), sizeof(DataPengaturan) + (18 * sizeof(JamDanMenit)) + 1))
              {
                bluetoothApk.responData(&bluetoothApk.responOK, 1);
                tanggalSebelumnya = 0;
              }
              else
              {
                bluetoothApk.responData(&bluetoothApk.responNull, 1);
              }
              break;
            case simpanTampilanJadwal:
              if (bluetoothApk.ambilDataEEPROM(offsetof(Setting, lamaTampilWaktuShalat), 41))//set jumlah data
              {
                bluetoothApk.responData(&bluetoothApk.responOK, 1);
                fungsiUpdateTampilan();
              }
              else
              {
                bluetoothApk.responData(&bluetoothApk.responNull, 1);
              }
              break;
            case simpanTampilanShalat:
              if (bluetoothApk.ambilDataEEPROM(offsetof(Setting, aktifAlarm), 72))//set jumlah data
              {
                bluetoothApk.responData(&bluetoothApk.responOK, 1);
                tanggalSebelumnya = 0;
              }
              else
              {
                bluetoothApk.responData(&bluetoothApk.responNull, 1);
              }
              break;
            case simpanTampilanInformasi:
              if (bluetoothApk.ambilDataEEPROM(offsetof(Setting, fontPesanTartil), 58))//set jumlah data
              {
                bluetoothApk.responData(&bluetoothApk.responOK, 1);
              }
              else
              {
                bluetoothApk.responData(&bluetoothApk.responNull, 1);
              }
              break;
            case simpanPengaturanUmum:
              if (bluetoothApk.ambilDataEEPROM(offsetof(Setting, rumahIbadah), 33))//set jumlah data
              {
                bluetoothApk.responData(&bluetoothApk.responOK, 1);
              }
              else
              {
                bluetoothApk.responData(&bluetoothApk.responNull, 1);
              }
              break;
            case simpanPengaturanWaktu:
              if (bluetoothApk.ambilDataEEPROM(offsetof(Setting, weekerAktif), 15))//set jumlah data
              {
                bluetoothApk.responData(&bluetoothApk.responOK, 1);
              }
              else
              {
                bluetoothApk.responData(&bluetoothApk.responNull, 1);
              }
              break;
          }
          break;
        case perintah:
          if (bluetoothApk.ambilBuffer(buffer, sizeof(buffer)))
          {
            switch (bluetoothApk.data.parameter)
            {
              case perintahNyala:
                modeOperasi = modeInformasi;
                dmd.setBrightness(25.5 * EEPROM.read(offsetof(Setting, kecerahanRunningText) + i));
                tanggalSebelumnya = 0;
                break;
              case perintahPadam:
                modeOperasi = modePadam;
                dmd.end();
                dmd.clearScreen();
                break;
              case perintahKecerahan:
                dmd.setBrightness(25.5 * buffer[0]);
                EEPROM.write(offsetof(Setting, kecerahanStandar), buffer[0]);
                break;
              case perintahDemo:
                setModeDemo();
                break;
              case perintahSettingAwal:
                pengaturanAwal();
                tanggalSebelumnya = 0;
                break;
              case perintahJumlahPanel:
                EEPROM.write(offsetof(Setting, jumlahPanel), buffer[0]);
                fungsiHapusEfek();
                fungsiUpdateTampilan();
                break;
              case perintahMainkanMusik:
                digitalWrite(pinRelayAmpli, relayOn);
                delay(500);
                mp3_play(buffer[0]);
                break;
              case perintahMusikBerhenti:
                mp3_stop();
                delay(200);
                digitalWrite(pinRelayAmpli, !relayOn);
                break;
              case perintahSetWaktu:
                modeOperasi = modeInformasi;
                rtc.adjust(*ptrDateTime);
                tanggalSebelumnya = 0;
                break;
              case perintahMulaiTartil:
                if (!hitungMundurTartil)
                {
                  hitungMundurTartil = 10 * 60;
                  initHitungMundurTartil = true;
                }
              case perintahMulaiTarhim:
                if (!hitungMundurTarhim)
                {
                  hitungMundurTarhim = 5 * 60;
                  initHitungMundurTarhim = true;
                }
                if (!hitungMundurAlarmMasukWaktuShalat)
                {
                  hitungMundurAlarmMasukWaktuShalat = EEPROM.read(offsetof(Setting, aktifAlarmWaktuMasuk)) * 5;
                  initHitungMundurAlarmMasukWaktuShalat = true;
                }

                if(!hitungMundurPengingat)
                {
                  hitungMundurPengingat = EEPROM.read(offsetof(Setting, jarakAlarm) + jadwalBerikutnya.index) * 60;
                  if(hitungMundurPengingat)
                  {
                    initHitungMundurPengingat = true;
                    hitungMundurAlarm = EEPROM.read(offsetof(Setting, jumlahAlarm));
                  }
                }
              case perintahMulaiAzan:
                hitungMundurTarhim = 0;
                if (!hitungMundurAzan)
                {
                  hitungMundurAzan = 5 * 60;
                  initHitungMundurAzan = true;
                }
              case perintahMulaiIqamah:
                hitungMundurTarhim = 0;
                hitungMundurAzan = 0;
                pesanSetelahAzan = false;
                if (!hitungMundurIqamah)
                {
                  hitungMundurIqamah = EEPROM.read(offsetof(Setting, lamaIqamah) + jadwalBerikutnya.index) * 60; //menit
                  initHitungMundurIqamah = true;

                  if (EEPROM.read(offsetof(Setting, aktifAlarmIqamah) + jadwalBerikutnya.index))
                    hitungMundurAlarmIqamah = EEPROM.read(offsetof(Setting, jumlahAlarmIqamah));
                  else
                    hitungMundurAlarmIqamah = 0;

                  if (!hitungMundurIqamah)
                  {
                    hitungMundurIqamah = hitungMundurAlarmIqamah;
                  }
                }
              case perintahMulaiShalat:
                modeOperasi = modeShalat;
                fungsiHapusEfek();
                hitungMundurTarhim = 0;
                hitungMundurAzan = 0;
                pesanSetelahAzan = false;
                hitungMundurIqamah = 0;
                if (!hitungMundurShalat)
                {
                  hitungMundurShalat = 20 * 60;
                  initHitungMundurShalat = true;
                }
                break;

              case perintahSelesaiShalat:
                if (hitungMundurShalat)
                {
                  hitungMundurShalat = 1;
                }
              case perintahSelesaiIqamah:
                if (hitungMundurIqamah)
                {
                  if (EEPROM.read(offsetof(Setting, aktifAlarmIqamah) + jadwalBerikutnya.index))
                    hitungMundurAlarmIqamah = EEPROM.read(offsetof(Setting, jumlahAlarmIqamah));
                  else
                    hitungMundurAlarmIqamah = 0;

                  hitungMundurIqamah = hitungMundurAlarmIqamah;
                }
              case perintahSelesaiAzan:
                if (hitungMundurAzan)
                {
                  hitungMundurAzan = 1;
                }
              case perintahSelesaiTarhim:
                if (hitungMundurTarhim)
                {
                  hitungMundurTarhim = 1;
                }
              case perintahSelesaiTartil:
                if (hitungMundurTartil)
                {
                  hitungMundurTartil = 1;
                }
                break;
            }
            bluetoothApk.responData(&bluetoothApk.responOK, 1);
            break;
          }
      }
    }
    if (modeOperasi != modePadam)
    {
      dmd.begin();
    }

  }
}

#endif
