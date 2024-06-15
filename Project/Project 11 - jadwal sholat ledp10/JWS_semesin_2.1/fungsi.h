#ifndef FUNGSI_H
#define FUNGSI_H

#include "setting.h"
#include "konstanta.h"
//#include "jadwal.h"
#include <RTC_Semesin.h>

#define epochHijriah          1948439.5f //math.harvard.edu
#define hariPerTahunHijriah   354.365f   //math.harvard.edu
#define hariPerPerideHijriah  (hariPerTahunHijriah * 30.0)


#define swap(value) asm volatile("swap %0" : "=r" (value) : "0" (value))
template <typename type> type sign(type value) {
  return type((value > 0) - (value < 0));
}

#define skala2kecepatanRunning(skala)   ((10 - skala) * skalaKecepatan) + kecepatanMinimal
#define lamaTampilDetik(detik)   (1.0 * detik * 1000L)/kecepatanRunningEfek

uint16_t jumlahHariPerBulanHijriah[] = {0,30,59,89,118,148,177,207,236,266,295,325,354};
uint16_t jumlahHariPerBulanMasehi[]={0,31,59,90,120,151,181,212,243,273,304,334};
double tahunBulan = 354.367068;


double get_julian_date(Tanggal tanggal)
{
  if (tanggal.bulan <= 2)
  {
    tanggal.tahun -= 1;
    tanggal.bulan += 12;
  }

  double a = floor(tanggal.tahun / 100.0);
  double b = 2 - a + floor(a / 4.0);

  if (tanggal.tahun < 1583)
    b = 0;
  if (tanggal.tahun == 1582) {
    if (tanggal.bulan > 10)
      b = -10;
    if (tanggal.bulan == 10) {
      b = 0;
      if (tanggal.tanggal > 4)
        b = -10;
    }
  }

  return floor(365.25 * (tanggal.tahun + 4716)) + floor(30.6001 * (tanggal.bulan + 1)) + tanggal.tanggal + b - 1524.5;
}

double konversiTanggalHijriahKeJulianDate(uint16_t tahun, uint8_t bulan, uint8_t tanggal)
{
  return (epochHijriah + tanggal + ceil(29.5 * (bulan - 1)) + (354L * (tahun - 1)) + floor((3 + (11 * tahun)) / 30)) - 1;
}

void masehiKeHijriah(DateTime masehi, JamDanMenit waktuSholatMagrib, int8_t koreksiHijriah, Tanggal &hijriah)
{
  uint16_t sisaHari;

  Tanggal tanggal = {masehi.tanggal, masehi.bulan, masehi.tahun};
  double julianDate = get_julian_date(tanggal);
 
  uint16_t menitMagrib = waktuSholatMagrib.jam * 60  + waktuSholatMagrib.menit;
  uint16_t menitSekarang = masehi.jam * 60 + masehi.menit;
 
  if (menitSekarang >= menitMagrib)
  {
    julianDate++;//Pergantian hari hijrah pada magrib
  }
 
  julianDate = 0.5 + floor(julianDate) + koreksiHijriah;
 
  Tanggal tanggalHijriah;
 
  hijriah.tahun = floor(((30 * (julianDate - epochHijriah)) + 10646) / 10631);
  hijriah.bulan = min(12.0, ceil((julianDate - (29 + konversiTanggalHijriahKeJulianDate(hijriah.tahun, 1, 1))) / 29.5) + 1);
  hijriah.tanggal = (julianDate - konversiTanggalHijriahKeJulianDate(hijriah.tahun, hijriah.bulan, 1)) + 1;
 
}
uint32_t jumlahHariDariTanggal(DateTime now)
{
  byte tahun = now.tahun - 2000;
  uint32_t jumlahHari = (uint32_t)tahun * 365;
  for (uint16_t i = 0; i < tahun; i++) 
  {
    if (!(i%4))
    {
      jumlahHari++;
    }
  }
  jumlahHari += jumlahHariPerBulanMasehi[now.bulan-1];
  if ( (now.bulan > 2) && !(tahun % 4)) 
  { 
    jumlahHari++;
  }
  jumlahHari+= now.tanggal;
  return jumlahHari + 259;// base 18 April 1999
}

void masehiKeHijriah(uint32_t HariMasehi, Tanggal *tanggal)
{
  uint16_t sisaHari;
  tanggal->tahun = floor(HariMasehi/tahunBulan);
  sisaHari = HariMasehi - (tahunBulan * tanggal->tahun);
 
  for(byte i=0;i<sizeof(jumlahHariPerBulanHijriah);i++)
  {
    if(sisaHari <= jumlahHariPerBulanHijriah[i])
    {
      sisaHari -= jumlahHariPerBulanHijriah[i-1];
      tanggal->bulan = i;
      break;
    }
  }
  tanggal->tanggal = sisaHari;
  tanggal->tahun += 1420;
}

void ambilHijriah(DateTime now, Tanggal *hijriah)
{
  uint32_t jumlahHari;

  jumlahHari = jumlahHariDariTanggal(now) + EEPROM.read(offsetof(Setting, koreksiHijriah));
//  uint8_t jadwalJam = EEPROM.read(offsetJadwalEEPROM + (Magrib * 2) + 0);
//  uint8_t jadwalMenit = EEPROM.read(offsetJadwalEEPROM + (Magrib * 2) + 1);
//
//  uint16_t unixWaktu = (now.jam * 60) + now.menit;
//  uint16_t unixMagrib = (jadwalJam * 60) + jadwalMenit;
//  if (unixWaktu >= unixMagrib)
//  {
//    jumlahHari++;//Pergantian hari hijrah pada magrib
//  }
  masehiKeHijriah(jumlahHari, hijriah);
}


byte bin2bcd(byte val)
{
  return val + 6 * (val / 10);
}

void bcdToArray(char *buf, byte nilai)
{
  swap(nilai);
  *buf++ = (nilai & 0x0f) + 0x30;
  swap(nilai);
  *buf++ = (nilai & 0x0f) + 0x30;
  *buf = 0;
  return 2;
}

void bufferJamMenitDetik(char *buf, uint8_t bin1, uint8_t bin2, int8_t bin3, char pembatas = ':');
void bufferJamMenitDetik(char *buf, uint8_t bin1, uint8_t bin2, int8_t bin3, char pembatas)
{
  byte bcd1 = bin2bcd(bin1);
  byte bcd2 = bin2bcd(bin2);
  byte bcd3 = bin2bcd(bin3);

  bcdToArray(buf + 0, bcd1);
  buf[2] = pembatas;
  bcdToArray(buf + 3, bcd2);
  if (bin3 == -1)
  {
    buf[5] = 0;
  }
  else
  {
    buf[5] = pembatas;
    bcdToArray(buf + 6, bcd3);
    buf[8] = 0;
  }
}



#endif
