/*
   JWS Semesin 1 - 8 panel

   Fitur yang tidak aktif
   1. Tampilan tanggal Hijriah
   2. Tampilan tanggal Jawa
   3. Tampilan tanggal Minang
   4. Pesan selama tartil
   5. Pesan selama tarhim
   6. Pesan selama Iqamah

*/

#define modeDemo                          false
#define perlindunganPassword              false
#define serialDebug                       0         //0-2

//defenisi pin
#define pinMP3Busy                        2
#define pinRelayAmpli                     3
#define pinBuzzer                         4
#define RTCDetikIRQ                       A3

#define relayOn                           LOW
#define buzzerOn                          HIGH

#define I2CEEPROM_ADDRESS                 0x57

//setting
#define periodaAlarmWaktuShalatMasuk      100//milidetik
#define periodaAlarmWaktuAkhirIqamah      300//milidetik

#define pixelLebarPanel                   32
#define pixelTinggiPanel                  16

//variabel Setting
#define kecepatanMinimal                  10
#define skalaKecepatan                    10

#define tokenSetting                      21
#define lamaAksesApk                      10L * 60 * 1000

#define jumlahNyalaPadam                  5
#define jumlahTextInformasi               10

#define jumlahWeeker                      60

#define fontNamaMesjid                    Arial14
#define fontJamDanMenitUtama              angka6x14
#define fontSimbolGambar                  simbolGambar

#include <Wire.h>
#include <EEPROM.h>
#include <DMD_Semesin.h>
#include <RTC_Semesin.h>
#include <DFPlayer_Mini_Mp3.h>
#include <BluetoothApk.h>

#include <fonts/angka6x14.h>
#include <fonts/SystemFont5x7Ramping.h>
#include <fonts/Arial14.h>
#include <fonts/simbolGambar.h>


#include "konstanta.h"
#include "setting.h"
#include "WaktuShalat.h"
#include "bluetooth.h"
#include "tampilan.h"
#include "jadwal.h"
#include "fungsi.h"

const uint8_t *alamatFont[] = {
  Arial14,
  SystemFont5x7Ramping,
};

//Status
bool RTCValid = true;
bool statusAlarm;
Tanggal Hijriah;

bool tampilJamMenitDetik = false;
uint32_t millisRunningText;
uint8_t kecepatanRunningText;
uint8_t kecepatanRunningEfek;

uint8_t hitungMundurWeeker;
uint32_t millisWeeker;
byte weekerStatus;

int8_t indexWaktuIbadah;
int8_t indekInformasi;

//efek
uint16_t lamaTampilText;
bool initTampil;

bool tampilanInformasiSambung;

byte indekTampilan = jumlahInformasi;
EfekMarque efekMarque;

uint16_t lebarText;
uint8_t tinggiText;

uint32_t millisAlarm;
uint32_t millisMasukWaktu;
bool status;

byte modeTampilanJadwal;
bool runningTextAktif;
bool efekAktif;

bool statusRelayAmpli;

void setup() 
{

  pinMode(pinBuzzer, OUTPUT);
  digitalWrite(pinRelayAmpli, !relayOn);
  pinMode(pinRelayAmpli, OUTPUT);
  pinMode(pinMP3Busy, INPUT_PULLUP);

  Serial.begin(9600);
  Serial.println(F("JWS Semesin 1-8 panel"));
  Serial.println(F("https://www.semesin.com/project"));

  mp3_set_serial (Serial);
  mp3_set_volume (15);


#if serialDebug > 1
  Serial.println(F("Inisialisasi"));
#endif

  rtc.begin();

  if (rtc.lostPower())
  {
#if serialDebug > 1
  Serial.println(F("RTC tidak jalan"));
#endif
    write_i2c_register(DS3231_ADDRESS, DS3231_STATUSREG, 0x00);
  }
  write_i2c_register(DS3231_ADDRESS, DS3231_CONTROL, DS3231_SquareWave1Hz);

  if (EEPROM.read(offsetof(Setting, token)) != tokenSetting)
  {
    pengaturanAwal();
  }

  fungsiUpdateTampilan = updateTampilan;
  fungsiHapusEfek = hapusEfek;
  fungsiUpdateTampilan();
  
  dmd.waitInterruptOver = false;
  dmd.begin();
  dmd.clearScreen();
  dmd.setBrightness(25.5 * EEPROM.read(offsetof(Setting, kecerahanStandar)));

#if serialDebug > 1
  Serial.println(F("Sistem mulai"));
#endif

  char32 *ptr32 = (char32*)buffer;
  EEPROM.get(offsetof(Setting, namaRumahIbadah), ptr32->teks);
  
  dmd.selectFont(fontNamaMesjid);
  dmd.drawString(1, 1, buffer);

  delay(3000);
  dmd.clearScreen();

  statusRelayAmpli = digitalRead(pinMP3Busy);
}
void loop() 
{

  if (digitalRead(RTCDetikIRQ))
  {
    if (RTCValid)
    {
      RTCValid = false;
      now = rtc.now();

#if serialDebug > 0
      bufferJamMenitDetik(buffer, now.jam, now.menit, now.detik);
      Serial.println(buffer);
#endif
      //dikerjakan bila hari berubah
      if (tanggalSebelumnya != now.tanggal)
      {
        tanggalSebelumnya = now.tanggal;

        updateJadwal();
        ambilHijriah(now, &Hijriah);
#if modeDemo
        setModeDemo();
#endif
      }

      tampilanJamMenitUtama(dmd);


      //dikerjakan bila menit berubah
      if (menitSebelumnya != now.menit)
      {
        menitSebelumnya =  now.menit;

        uint16_t unixWaktu  = (now.jam * 60) + now.menit;

        if (jadwalBerikutnya.aktif)
        {
          if (jadwalBerikutnya.waktu == unixWaktu)
          {
            jadwalBerikutnya.aktif = false;
            modeOperasi = modeShalat;
            dmd.clearScreen();
            fungsiHapusEfek();
          }
        }
        if (weekerBerikutnya.aktif)
        {
          if (weekerBerikutnya.waktu == unixWaktu)
          {
            weekerBerikutnya.aktif = false;
            hitungMundurWeeker = jumlahWeeker;
          }
        }


        if (!((modeOperasi == modePadam) || (modeOperasi == modeShalat)))
        {
          for (byte i = 0; i < jumlahNyalaPadam; i++)
          {
            if (
              EEPROM.read(offsetof(Setting, AktifNyalaPadam) + (i * sizeof(bool))) &&
              (now.jam == EEPROM.read(offsetof(Setting, padam) + (i * sizeof(JamDanMenit)) + offsetof(JamDanMenit, jam))) &&
              (now.menit == EEPROM.read(offsetof(Setting, padam) + (i * sizeof(JamDanMenit)) + offsetof(JamDanMenit, menit)))
            )
            {
              modeOperasi = modePadam;
              dmd.clearScreen();
              dmd.end();
#if serialDebug > 1
              Serial.println(F("Mode Padam"));
#endif
            }
          }
        }

        for (byte i = 0; i < jumlahNyalaPadam; i++)
        {
          if (
            EEPROM.read(offsetof(Setting, AktifNyalaPadam) + (i * sizeof(bool))) &&
            (now.jam == EEPROM.read(offsetof(Setting, nyala) + (i * sizeof(JamDanMenit)) + offsetof(JamDanMenit, jam))) &&
            (now.menit == EEPROM.read(offsetof(Setting, nyala) + (i * sizeof(JamDanMenit)) + offsetof(JamDanMenit, menit)))
          )
          {
            modeOperasi = modeInformasi;
            updateJadwal();

#if serialDebug > 1
            Serial.println(F("Mode Normal"));
#endif
            dmd.setBrightness(25.5 * EEPROM.read(offsetof(Setting, kecerahanRunningText) + i));
            dmd.begin();
          }
        }
      }

      //Timing detik
      if (modeOperasi == modeShalat)
      {
        prosedurShalat();
      }

      if (modeOperasi == modeInformasi)
      {
        prosedurInformasi();
      }
      if(hitungMundurWeeker)
      {
        hitungMundurWeeker--;
        if(!hitungMundurWeeker)
        {
          cekWeeker();
        }
      }
    }
  }
  else
  {
    RTCValid = true;
  }

  if((modeOperasi == modeShalat) && hitungMundurPengingat)
  {
    if(millisMasukWaktu < millis())
    {
      clearTampilanJadwal(dmd);
    }
  }

  if(hitungMundurWeeker)
  {
    if(millisWeeker < millis())
    {
      switch(weekerStatus)
      {
        case 0:
        case 2:
        case 4:
          digitalWrite(pinBuzzer, buzzerOn);
          millisWeeker = millis() + 100;
          break;
        case 1:
        case 3:
          digitalWrite(pinBuzzer, !buzzerOn);
          millisWeeker = millis() + 100;
          break;
        case 5:
          digitalWrite(pinBuzzer, !buzzerOn);
          millisWeeker = millis() + 500;
          break;
      }
      weekerStatus++;
      weekerStatus %= 6;
    }
  }
  else if (millisAlarm < millis())
  {
    digitalWrite(pinBuzzer, LOW);
  }

  if (digitalRead(pinMP3Busy))
  {
    if (!statusRelayAmpli)
    {
      statusRelayAmpli = true;
      delay(500);
      digitalWrite(pinRelayAmpli, !relayOn);
    }
  }
  else
  {
    statusRelayAmpli = false;
  }

  if (runningTextAktif)
  {
    if (millisRunningText < millis())
    {
      millisRunningText = millis() + kecepatanRunningText;

      if (efekMarque.mode == XMinus)
      {
        dmd.marqueeXMinus(&efekMarque);
      }
      if(!efekMarque.mode)
      {
        runningTextAktif = false;
        if ((modeOperasi == modeInformasi) && (tampilanInformasiSambung))
        {
          prosedurInformasi();
        }
      }
    }
  }
  cekBluetooth();
  delay(1);
}
void prosedurInformasi()
{
labelProsedurInformasi:
  if (runningTextAktif || efekAktif)
  {

  }
  else if ((indekTampilan == informasiText) && tampilanInformasiSambung)
  {
    initTampil = true;
  }
  else if ((indekTampilan >= informasiTetap) && (indekTampilan < informasiText))
  {
    initTampil = true;
    indekTampilan++;
  }
  else
  {
    if (lamaTampilText)
    {
      lamaTampilText--;
    }
    else
    {
      initTampil = true;
      if (++indekTampilan >= jumlahInformasi)
      {
        indekTampilan = 0;
      }
      if ((indekTampilan == informasiWaktu) && (EEPROM.read(offsetof(Setting, jumlahPanel)) != 1))
      {
        indekTampilan++;
      }
      if ((indekTampilan == informasiTanggalMasehi1) && !EEPROM.read(offsetof(Setting, tampilanMasehi)))
      {
        indekTampilan+=2;
      }
    }
  }

#if serialDebug > 0
  Serial.print(F("indekTampilan = "));
  Serial.println(indekTampilan);
#endif

  if (initTampil)
  {
    initTampil = false;

    if(indekTampilan == informasiTetap)
    {
      if (EEPROM.read(offsetof(Setting, aktifPesanInformasiTetap)))
      {
        byte indexFont = EEPROM.read(offsetof(Setting, fontPesanInformasiTetap));
        uint16_t I2CEEPROMAlamat;
        EEPROM.get(offsetof(Setting, alamatPesanInformasiTetap), I2CEEPROMAlamat);
        byte kecepatan = EEPROM.read(offsetof(Setting, kecepatanPesanInformasiTetap));
        tampilanRunningText(0, I2CEEPROMAlamat, kecepatan, true);

        bool adaInformasi = false;
        for (byte i = 0; i < jumlahTextInformasi; i++)
        {
          if (EEPROM.read(offsetof(Setting, aktifInformasi) + i))
          {
            adaInformasi = true;
            break;
          }
        }
        if(!adaInformasi)
        {
          efekMarque.clear = 64;
        }
      }
      else
      {
        goto labelProsedurInformasi;
      }

    }
    else if (indekTampilan == informasiText)
    {
      if (!tampilanInformasiSambung)
      {
        clearTampilanJadwal(dmd);
      }

      bool adaInformasi = false;
      byte sambung = false;
      int idx = indekInformasi + 1;
      if (idx == jumlahTextInformasi)
      {
        idx = 0;
      }

      for (byte i = 0; i < jumlahTextInformasi; i++)
      {

        if (EEPROM.read(offsetof(Setting, aktifInformasi) + (idx * sizeof(bool))))
        {
          indekInformasi = idx;
          adaInformasi = true;
        }
        idx++;
        if (idx == jumlahTextInformasi)
        {
          idx = 0;
        }

        if (adaInformasi)
        {

          sambung = EEPROM.read(offsetof(Setting, sambungInformasi) + (idx * sizeof(bool)));
          break;
        }
      }

#if serialDebug > 0
      Serial.print(F("indekInformasi = "));
      Serial.println(indekInformasi);
#endif

      if (adaInformasi)
      {
        byte indexFont = EEPROM.read(offsetof(Setting, fontInformasi) + (indekInformasi * sizeof(uint8_t)));
        uint16_t I2CEEPROMAlamat;
        EEPROM.get(offsetof(Setting, alamatInformasi) + (indekInformasi * sizeof(uint16_t)), I2CEEPROMAlamat);
        byte kecepatan = EEPROM.read(offsetof(Setting, kecepatanInformasi) + (indekInformasi * sizeof(uint8_t)));
        tampilanRunningText(indexFont, I2CEEPROMAlamat, kecepatan, sambung);

      }
      else
      {
        tampilanInformasiSambung = false;
        goto labelProsedurInformasi;
      }
    }
    else
    {
      clearTampilanJadwal(dmd);
      lamaTampilText = EEPROM.read(offsetof(Setting, lamaTampilWaktuShalat));
      memset(buffer, 0, sizeof(buffer));

      if (indekTampilan == informasiPembuka)
      {
        strcpy_P(buffer, strJadwalShalat);
        tampilanJadwal(dmd);
      }
      else if (indekTampilan == informasiWaktu)
      {
        dmd.selectFont(fontJamDanMenitUtama);
        bufferJamMenitDetik(buffer, now.jam, now.menit, -1);
        lebarText = dmd.stringWidth(buffer);
        dmd.drawString(0, 1, buffer);
      }
      else if (indekTampilan == informasiTanggalMasehi1)
      {
        if(tampilanTanggal(strNamaHariMasehi[now.hari - 1], strNamaBulanMasehi[now.bulan - 1], now, dmd, true))
        {
          indekTampilan++;
        }
      }
      else if (indekTampilan == informasiTanggalMasehi2)
      {
        tampilanTanggal(strNamaHariMasehi[now.hari - 1], strNamaBulanMasehi[now.bulan - 1], now, dmd, false);
      }
      else if (indekTampilan <= informasiIsya)//tampilkan waktu-waktu sholat
      {
        for (byte i = indekTampilan; i <= informasiIsya; i++)
        {
          if ((indekTampilan == informasiImsak) && !EEPROM.read(offsetof(Setting, tampilanImsak)))
          {
            indekTampilan++;
          }
          if ((indekTampilan == informasiTerbit) && !EEPROM.read(offsetof(Setting, tampilanTerbit)))
          {
            indekTampilan++;
          }
          if ((indekTampilan == informasiDuha) && !EEPROM.read(offsetof(Setting, tampilanDuha)))
          {
            indekTampilan++;
          }
          if ((indekTampilan == informasiTerbenam) && !EEPROM.read(offsetof(Setting, tampilanTerbenam)))
          {
            indekTampilan++;
          }
        }
        int8_t indexNamaShalat = indekTampilan - informasiImsak;

        if (indexNamaShalat >= 0)
        {
          strcpy_P(buffer, strWaktuIbadah[indexNamaShalat]);
          *(buffer + strlen(buffer)) = ' ';

          byte jadwalJam = EEPROM.read(offsetJadwalEEPROM + (indexNamaShalat * 2) + 0);
          byte jadwalMenit = EEPROM.read(offsetJadwalEEPROM + (indexNamaShalat * 2) + 1);
          bufferJamMenitDetik(buffer + strlen(buffer), jadwalJam, jadwalMenit, -1);
          tampilanJadwal(dmd);
        }
      }
    }
  }
}
void prosedurShalat()//dalam loop detik
{
  if (hitungMundurTartil)
  {
#if serialDebug > 1
    Serial.print(F("hitungMundurTartil="));
    Serial.println(hitungMundurTartil);
#endif
    if (initHitungMundurTartil)
    {
      initHitungMundurTartil = false;
      clearTampilanJadwal(dmd);
      digitalWrite(pinRelayAmpli, relayOn);
      delay(500);
      mp3_play (jadwalBerikutnya.suaraTartil);
    }

    hitungMundurTartil--;
    if (!hitungMundurTartil)
    {
      mp3_stop();
      if (!jadwalBerikutnya.suaraTarhim)
      {
        delay(200);
        digitalWrite(pinRelayAmpli, !relayOn);
      }
    }

    if (!hitungMundurAlarm)
    {
      memset(buffer, 0, sizeof(buffer));
      strcpy_P(buffer, strTartil);
      *(buffer + strlen(buffer)) = ' ';
      bufferJamMenitDetik(buffer + strlen(buffer), hitungMundurTartil / 60, hitungMundurTartil % 60, -1);
      tampilanJadwal(dmd);
    }
  }
  else if (hitungMundurTarhim)
  {
#if serialDebug > 1
    Serial.print(F("hitungMundurTarhim="));
    Serial.println(hitungMundurTarhim);
#endif
    if (initHitungMundurTarhim)
    {
      initHitungMundurTarhim = false;
      clearTampilanJadwal(dmd);

      mp3_play (jadwalBerikutnya.suaraTarhim);
    }

    hitungMundurTarhim--;
    if (!hitungMundurTarhim)
    {
      mp3_stop();
      delay(200);
      digitalWrite(pinRelayAmpli, !relayOn);
    }

    if (!hitungMundurAlarm)
    {
      memset(buffer, 0, sizeof(buffer));
      strcpy_P(buffer, strTarhim);
      *(buffer + strlen(buffer)) = ' ';
      bufferJamMenitDetik(buffer + strlen(buffer), hitungMundurTarhim / 60, hitungMundurTarhim % 60, -1);
      tampilanJadwal(dmd);
    }
  }
  else if (hitungMundurPengingat)
  {
    if (initHitungMundurPengingat)
    {
      dmd.clearScreen();
      tampilanJamMenitUtama(dmd);
      initHitungMundurPengingat = false;
    }
    tampilanHitungMundur(dmd, strWaktuIbadah[jadwalBerikutnya.index], hitungMundurPengingat);
    hitungMundurPengingat--;

    millisMasukWaktu = millis() + 500;
    
    if(hitungMundurAlarm)
    {
      hitungMundurAlarm--;
      digitalWrite(pinBuzzer, HIGH);
      millisAlarm = millis() + periodaAlarmWaktuShalatMasuk;
    }
    if(hitungMundurPengingat <= hitungMundurAlarmMasukWaktuShalat)
    {
      hitungMundurPengingat = 0;
    }
    
  }
  else if(hitungMundurAlarmMasukWaktuShalat)
  {
    if(initHitungMundurAlarmMasukWaktuShalat)
    {
      initHitungMundurAlarmMasukWaktuShalat = false;
    }
    tampilanHitungMundur(dmd, strWaktuIbadah[jadwalBerikutnya.index], hitungMundurAlarmMasukWaktuShalat);
    hitungMundurAlarmMasukWaktuShalat--;

    digitalWrite(pinBuzzer, HIGH);
    if(hitungMundurAlarmMasukWaktuShalat > 2)
    {
      millisAlarm = millis() + 500;
    }
    else
    {
      millisAlarm = millis() + 2000;
    }
  }
  else if (hitungMundurAzan)
  {
#if serialDebug > 1
    Serial.print(F("hitungMundurAzan="));
    Serial.println(hitungMundurAzan);
#endif
    if (initHitungMundurAzan)
    {
      initHitungMundurAzan = false;
      clearTampilanJadwal(dmd);
    }
    tampilanHitungMundur(dmd, strAzan, hitungMundurAzan);
    hitungMundurAzan--;
  }
  else if (pesanSetelahAzan)
  {
#if serialDebug > 1
    Serial.print(F("pesanSetelahAzan="));
    Serial.println(pesanSetelahAzan);
#endif
    pesanSetelahAzan = false;

    byte indexFont = EEPROM.read(offsetof(Setting, fontPesanSetelahAzan));
    uint16_t I2CEEPROMAlamat;
    EEPROM.get(offsetof(Setting, alamatPesanSetelahAzan), I2CEEPROMAlamat);
    byte kecepatan = EEPROM.read(offsetof(Setting, kecepatanPesanSetelahAzan));
    tampilanRunningText(indexFont, I2CEEPROMAlamat, kecepatan, false);

  }
  else if (hitungMundurIqamah)
  {
#if serialDebug > 1
    Serial.print(F("hitungMundurIqamah="));
    Serial.println(hitungMundurIqamah);
#endif
    hitungMundurIqamah--;
    if ((!runningTextAktif) || (hitungMundurIqamah <= hitungMundurAlarmIqamah) || (hitungMundurIqamah <= 15))
    {
      fungsiHapusEfek();
      if (initHitungMundurIqamah)
      {
        initHitungMundurIqamah = false;
        clearTampilanJadwal(dmd);
      }
      tampilanHitungMundur(dmd, strIqamah, hitungMundurIqamah);

      if (hitungMundurIqamah < hitungMundurAlarmIqamah)
      {
        digitalWrite(pinBuzzer, HIGH);
        millisAlarm = millis() + periodaAlarmWaktuAkhirIqamah;
      }
    }
  }
  else if (pesanSebelumShalat)
  {
#if serialDebug > 1
    Serial.print(F("pesanSebelumShalat="));
    Serial.println(pesanSebelumShalat);
#endif
    pesanSebelumShalat = false;

    byte indexFont = EEPROM.read(offsetof(Setting, fontPesanSebelumShalat));
    uint16_t I2CEEPROMAlamat;
    EEPROM.get(offsetof(Setting, alamatPesanSebelumShalat), I2CEEPROMAlamat);
    byte kecepatan = EEPROM.read(offsetof(Setting, kecepatanPesanSebelumShalat));
    tampilanRunningText(indexFont, I2CEEPROMAlamat, kecepatan, false);
  }
  else if (hitungMundurPeringatanSimbol)
  {
#if serialDebug > 1
    Serial.print(F("hitungMundurPeringatanSimbol="));
    Serial.println(hitungMundurPeringatanSimbol);
#endif
    if (!runningTextAktif)
    {
      if (initHitungMundurPeringatanSimbol)
      {
        initHitungMundurPeringatanSimbol = false;
        clearTampilanJadwal(dmd);

        byte index = 0;

        if (EEPROM.read(offsetof(Setting, simbolKakiBersentuhan)))
        {
          buffer[index++] = 33;
        }
        if (EEPROM.read(offsetof(Setting, simbolHP)))
        {
          buffer[index++] = 34;
        }
        if (EEPROM.read(offsetof(Setting, simbolDilarangBerisik)))
        {
          buffer[index++] = 35;
        }
        buffer[index++] = 0;

        dmd.selectFont(fontSimbolGambar);
        lebarText = dmd.stringWidth(buffer);
        dmd.drawString(((lebarJadwalShalat - lebarText) / 2) + lebarJamUtama + 1, 0, buffer);
      }
      hitungMundurPeringatanSimbol--;
    }
  }

  else// if (hitungMundurShalat)
  {
#if serialDebug > 1
    Serial.print(F("hitungMundurShalat="));
    Serial.println(hitungMundurShalat);
#endif
    if (initHitungMundurShalat)
    {
      initHitungMundurShalat = false;
      clearTampilanJadwal(dmd);
    }

    if (hitungMundurShalat)
    {
      hitungMundurShalat--;
    }
    if (!hitungMundurShalat)
    {
      modeOperasi = modeInformasi;
      cekJadwal();
    }
  }
}
void hapusEfek()
{
  tampilanInformasiSambung = false;
  efekMarque.mode = nonAktif;
  lamaTampilText = 0;
  indekTampilan = -1;
  indekInformasi = 0;
  runningTextAktif = false;
  hitungMundurWeeker = 0;
  digitalWrite(pinBuzzer, !buzzerOn);
  mp3_stop();
}
void clearTampilanJadwal(DMDFrame frame)
{
  frame.drawFilledBox(lebarJamUtama, 0, lebarJamUtama + lebarJadwalShalat, 15, GRAPHICS_OFF);
}
void tampilanJamMenitUtama(DMDFrame frame)
{
  byte jumlahPanel = EEPROM.read(offsetof(Setting, jumlahPanel));
  if (!(((jumlahPanel < 3) && runningTextAktif) || (jumlahPanel == 1)))
  {
    frame.selectFont(fontJamDanMenitUtama);
    frame.fontHeader.fixedWidth = 2;
    bufferJamMenitDetik(buffer, now.jam, now.menit, now.detik, now.detik % 2 ? ':' : ' ');
    lebarText = frame.stringWidth(buffer);
    if (lebarText > lebarJamUtama) //00:00:00
    {
      buffer[5] = 0;
    }
    frame.drawString(0, 1, buffer);
    frame.drawLine(lebarJamUtama - 1, 0, lebarJamUtama - 1, 15);
  }
}
void tampilanJadwal(DMDFrame frame)
{
  frame.selectFont(alamatFont[0]);
  lebarText = frame.stringWidth(buffer);
  tinggiText = frame.fontHeader.height;
  if (lebarText <= lebarJadwalShalat - 1) //Jadwal Shalat font besar
  {
    if (modeTampilanJadwal != 1)
    {
      clearTampilanJadwal(dmd);
    }
    modeTampilanJadwal = 1;

    *(buffer + strlen(buffer)) = ' ';
    frame.drawString(lebarJamUtama + 1, (frame.height - tinggiText) / 2, buffer);
  }
  else
  {
    frame.selectFont(alamatFont[1]);
    lebarText = frame.stringWidth(buffer);
    tinggiText = frame.fontHeader.height;
    if (lebarText <= lebarJadwalShalat - 1) //Jadwal Shalat font kecil
    {
      if (modeTampilanJadwal != 2)
      {
        clearTampilanJadwal(dmd);
      }
      modeTampilanJadwal = 2;
      *(buffer + strlen(buffer)) = ' ';
      frame.drawString(lebarJamUtama + 1, (frame.height - tinggiText) / 2, buffer);
    }
    else
    {
      if (modeTampilanJadwal != 3)
      {
        clearTampilanJadwal(dmd);
      }
      modeTampilanJadwal = 3;
      lebarText = frame.stringWidth(strchr(buffer, ' ') + 1);
      *(buffer + strlen(buffer)) = ' ';
      frame.drawString(((lebarJadwalShalat - lebarText) / 2) + lebarJamUtama, 8, strchr(buffer, ' ') + 1);
      char *ptr = strchr(buffer, ' ');
      *ptr = 0;
      lebarText = frame.stringWidth(buffer);
      *ptr++ = ' ';
      *ptr = 0;
      if (lebarText > lebarJadwalShalat)
      {
        frame.drawString(lebarJamUtama + 1, 0, buffer);
      }
      else
      {
        frame.drawString(((lebarJadwalShalat - lebarText) / 2) + lebarJamUtama, 0, buffer);
      }
    }
  }
}
void updateTampilan()
{
  byte jumlahPanel = EEPROM.read(offsetof(Setting, jumlahPanel));

  if (jumlahPanel > 4)
  {
    lebarJamUtama = 49;
  }
  else if (jumlahPanel > 1)
  {
    lebarJamUtama = 32;
  }
  else
  {
    lebarJamUtama = 0;
  }
  lebarJadwalShalat = (jumlahPanel * 32) - lebarJamUtama;
  dmd.widthBytesToShow = ((jumlahPanel * 32) + 7) / 8;
  kecepatanRunningEfek = skala2kecepatanRunning(EEPROM.read(offsetof(Setting, kecepatanEfek)));
}
bool tampilanTanggal(const char *namaHari, const char *namaBulan, DateTime tgl, DMDFrame frame, bool tampilHari)
{
  char *ptr;
  memset(buffer, 0, sizeof(buffer));
  strcpy_P(buffer, namaHari);
  *(buffer + strlen(buffer)) = ',';
  *(buffer + strlen(buffer)) = ' ';
  bcdToArray(buffer + strlen(buffer), bin2bcd(tgl.tanggal));
  *(buffer + strlen(buffer)) = ' ';
  strcpy_P(buffer + strlen(buffer), namaBulan);
  *(buffer + strlen(buffer)) = ' ';
  bcdToArray(buffer + strlen(buffer), bin2bcd(tgl.tahun / 100));
  bcdToArray(buffer + strlen(buffer), bin2bcd(tgl.tahun % 100));

#if serialDebug > 1
  Serial.println(buffer);
#endif
  frame.selectFont(alamatFont[0]);
  lebarText = frame.stringWidth(buffer);
  tinggiText = frame.fontHeader.height;
  if (lebarText <= lebarJadwalShalat) //sabtu, 01 Januari 2000 font besar
  {
    frame.drawString(lebarJamUtama + 1, (frame.height - tinggiText) / 2, buffer);
    return true;
  }
  else
  {
    frame.selectFont(alamatFont[1]);
    lebarText = frame.stringWidth(buffer);
    tinggiText = frame.fontHeader.height;
    if (lebarText <= lebarJadwalShalat)//sabtu, 01 Januari 2000 font kecil
    {
      frame.drawString(lebarJamUtama + 1, (frame.height - tinggiText) / 2, strchr(buffer, ' ') + 1);
      return true;
    }
    else if(tampilHari)
    {
      *(strchr(buffer, ',')) = 0;
      frame.selectFont(alamatFont[0]);
      lebarText = frame.stringWidth(buffer);
      if (lebarText > lebarJadwalShalat) //sabtu font kecil
      {
        frame.selectFont(alamatFont[1]);
        lebarText = frame.stringWidth(buffer);
      }
      tinggiText = frame.fontHeader.height;
      frame.drawString(lebarJamUtama + 1, (frame.height - tinggiText) / 2, buffer);
    }
    else
    {
      ptr = strchr(buffer, ' ') + 1;
      frame.selectFont(alamatFont[0]);
      lebarText = frame.stringWidth(ptr);
      tinggiText = frame.fontHeader.height;
      if (lebarText <= lebarJadwalShalat)//01 Januari 2000 font besar
      {
        frame.drawString(lebarJamUtama + 1, (frame.height - tinggiText) / 2, ptr);
      }
      else
      {
        frame.selectFont(alamatFont[1]);
        lebarText = frame.stringWidth(ptr);
        tinggiText = frame.fontHeader.height;
        if (lebarText <= lebarJadwalShalat)//01 Januari 2000 font kecil
        {
          frame.drawString(lebarJamUtama + 1, (frame.height - tinggiText) / 2, ptr);
        }
        else
        {
          memset(buffer, 0, sizeof(buffer));
          bcdToArray(buffer + strlen(buffer), bin2bcd(tgl.tanggal));
          *(buffer + strlen(buffer)) = '-';
          bcdToArray(buffer + strlen(buffer), bin2bcd(tgl.bulan));
          *(buffer + strlen(buffer)) = '-';
          bcdToArray(buffer + strlen(buffer), bin2bcd(tgl.tahun / 100));
          bcdToArray(buffer + strlen(buffer), bin2bcd(tgl.tahun % 100));
          
          frame.selectFont(alamatFont[0]);
          lebarText = frame.stringWidth(buffer);
          tinggiText = frame.fontHeader.height;
          if (lebarText <= lebarJadwalShalat) //01-01-0000 font besar
          {
            frame.drawString(lebarJamUtama + 1, (frame.height - tinggiText) / 2, buffer);
          }
          else
          {

            frame.selectFont(alamatFont[1]);
            lebarText = frame.stringWidth(buffer);
            tinggiText = frame.fontHeader.height;
            if (lebarText <= lebarJadwalShalat) //01-01-0000 font kecil
            {
              frame.drawString(lebarJamUtama + 1, (frame.height - tinggiText) / 2, buffer);
            }
            else
            {
                frame.selectFont(angka6x14);
                bcdToArray(buffer, bin2bcd(tgl.tanggal));
                tinggiText = frame.fontHeader.height;
                frame.drawString(lebarJamUtama + 1, (frame.height - tinggiText) / 2, buffer);
  
                frame.selectFont(alamatFont[1]);
                bcdToArray(buffer, bin2bcd(tgl.bulan));
                frame.drawString(lebarJamUtama + 16, 0, buffer);
                bcdToArray(buffer, bin2bcd(tgl.tahun % 100));
                frame.drawString(lebarJamUtama + 16 , 8, buffer);
                frame.drawLine(lebarJamUtama + 16, 7, lebarJamUtama + 16 + 11, 7);
            }
          }
        }
      }
    }
  }
  return false;
}
void tampilanHitungMundur(DMDFrame frame, char *str, uint16_t hitungMundur)
{    
  memset(buffer, 0, sizeof(buffer));
  strcpy_P(buffer, str);
  *(buffer + strlen(buffer)) = ' ';
  bufferJamMenitDetik(buffer + strlen(buffer), hitungMundur / 60, hitungMundur % 60, -1);
  tampilanJadwal(frame);
  
}    
void tampilanRunningText(byte indexFont, uint16_t I2CEEPROMAlamat, byte kecepatan, bool sambung)
{
  efekMarque.mode = XMinus;
  efekMarque.init = true;
  efekMarque.sumber = sumberI2CEEPROM0x57;

  efekMarque.font = (uint8_t*)alamatFont[indexFont];

  efekMarque.alamat = I2CEEPROMAlamat;
  byte jumlahPanel = EEPROM.read(offsetof(Setting, jumlahPanel));
  if (jumlahPanel < 3)
  {
    efekMarque.kiri = 0;
    efekMarque.lebar = lebarJamUtama + lebarJadwalShalat;
    if (!tampilanInformasiSambung)
      dmd.clearScreen();
  }
  else
  {
    efekMarque.kiri = lebarJamUtama;
    efekMarque.lebar = lebarJadwalShalat;
    if (!tampilanInformasiSambung)
      clearTampilanJadwal(dmd);
  }

  dmd.selectFont(alamatFont[indexFont]);
  tinggiText = dmd.fontHeader.height;

  efekMarque.atas = (dmd.height - tinggiText) / 2;
  efekMarque.tinggi = tinggiText;
  efekMarque.step = 1;
  efekMarque.skip = 0;
  if (sambung)
  {
    efekMarque.clear = 10;
  }
  else if (jumlahPanel < 3)
  {
    efekMarque.clear = lebarJamUtama + lebarJadwalShalat;
  }
  else
  {
    efekMarque.clear = lebarJadwalShalat;
  }

  tampilanInformasiSambung = sambung;
  kecepatanRunningText = skala2kecepatanRunning(kecepatan);
  runningTextAktif = true;
}
