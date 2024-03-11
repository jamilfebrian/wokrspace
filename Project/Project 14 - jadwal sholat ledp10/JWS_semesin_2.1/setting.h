#ifndef SETTING_H
#define SETTING_H

#include "konstanta.h"

byte detikSebelumnya = 60;
byte menitSebelumnya = 60;
byte tanggalSebelumnya = 0;
byte modeOperasi = modeInformasi;

byte lebarJamUtama = 32;
byte lebarJadwalShalat = 32;

struct JamDanMenit {
  uint8_t jam;
  uint8_t menit;
};


struct Tanggal {
  uint8_t tanggal;
  uint8_t bulan;
  uint16_t tahun;
};

enum JuristicMethod
{
  Shafii,    // Shafii (standard)
  Hanafi,    // Hanafi
};

struct char32 {
  char teks[32];
};
struct char16 {
  char teks[16];
};
struct char8 {
  char teks[9];
};

struct DataPengaturan
{
  int8_t zonaWaktu;
  int8_t metodeAsar;
  int8_t aturImsak;
  int8_t aturSubuh;
  int8_t aturTerbit;
  int8_t aturDuha;
  int8_t aturZuhur;
  int8_t aturAsar;
  int8_t aturTerbenam;
  int8_t aturMagrib;
  int8_t aturIsya;


  double lintang;
  double bujur;
  double ketinggian;
  double sudutSubuh;
  double sudutTerbit;
  double sudutDuha;
  double sudutMagrib;
  double sudutIsya;

  JamDanMenit jadwalMatahari[jumlahWaktuShalat];
};

struct Setting
{
  byte token;
  byte rumahIbadah;
  char32 namaRumahIbadah;
  char16 idPengguna;
  char8 kataSandi;

  byte jumlahPanel;
  byte kecerahanStandar;

  uint8_t lamaTampilWaktuShalat;
  uint8_t kecerahanRunningText[jumlahNyalaPadam];
  int8_t koreksiHijriah;

  bool AktifNyalaPadam[jumlahNyalaPadam];
  bool tampilanMasehi;
  bool tampilanHijriah;
  bool tampilanPasaranJawa;
  bool tampilanTanggalMinang;
  bool tampilanImsak;
  bool tampilanTerbit;
  bool tampilanDuha;
  bool tampilanTerbenam;

  uint8_t kecepatanEfek;

  JamDanMenit padam[jumlahNyalaPadam];
  JamDanMenit nyala[jumlahNyalaPadam];


  bool aktifAlarm[jumlahWaktuShalat];
  bool aktifAlarmIqamah[5];
  bool simbolHP;
  bool simbolKakiBersentuhan;
  bool simbolDilarangBerisik;
  bool aktifTartil[5];
  bool aktifTarhim[5];
  bool aktifAlarmWaktuMasuk;
  
  uint8_t jarakAlarm[jumlahWaktuShalat];
  uint8_t jumlahAlarm;
  uint8_t jumlahAlarmIqamah;
  uint8_t lamaAzan;
  uint8_t lamaTartil[5];
  uint8_t suaraTartil[5];
  uint8_t lamaTarhim[5];
  uint8_t suaraTarhim[5];
  uint8_t lamaIqamah[5];
  uint8_t lamaShalat[5];
  uint8_t lamaShalatJumat;
  uint8_t lamaSimbolGambar;//detik

  bool weekerAktif[5];
  JamDanMenit weeker[5];

  uint8_t fontPesanTartil;
  uint8_t fontPesanTarhim;
  uint8_t fontPesanSetelahAzan;
  uint8_t fontPesanSelamaIqamah;
  uint8_t fontPesanSebelumShalat;
  uint8_t fontPesanInformasiTetap;
  uint8_t fontInformasi[jumlahTextInformasi];

  uint8_t kecepatanPesanTartil;
  uint8_t kecepatanPesanTarhim;
  uint8_t kecepatanPesanSetelahAzan;
  uint8_t kecepatanPesanSelamaIqamah;
  uint8_t kecepatanPesanSebelumShalat;
  uint8_t kecepatanPesanInformasiTetap;
  uint8_t kecepatanInformasi[jumlahTextInformasi];

  bool aktifPesanTartil;
  bool aktifPesanTarhim;
  bool aktifPesanSetelahAzan;
  bool aktifPesanSelamaIqamah;
  bool aktifPesanSebelumShalat;
  bool aktifPesanInformasiTetap;
  bool aktifInformasi[jumlahTextInformasi];
  bool sambungInformasi[jumlahTextInformasi];

  JamDanMenit jadwalManual[jumlahWaktuShalat];
  JamDanMenit jadwalOnline[jumlahWaktuShalat];

  uint8_t modeJadwal;
  DataPengaturan dataPengaturan;

  uint16_t alamatPesanTartil;
  uint16_t alamatPesanTarhim;
  uint16_t alamatPesanSetelahAzan;
  uint16_t alamatPesanSelamaIqamah;
  uint16_t alamatPesanSebelumShalat;
  uint16_t alamatPesanInformasiTetap;
  uint16_t alamatInformasi[jumlahTextInformasi];

};


const Setting settingAwal PROGMEM =
{
  tokenSetting,                   //token
  0,                              //rumahIbadah
  "Masjid",                       //namaRumahIbadah
  "jws",                          //idPengguna
  "1234",                         //kataSandi

  3,                              //jumlahPanel
  5,                              //kecerahanStandar;

  5,                              //lamaTampilWaktuShalat
  {5, 5, 5, 5, 5},                //kecerahanRunningText
  0,                              //koreksiHijriah

  {false, false, false, false, false}, //AktifNyalaPadam
  true,                           //tampilanMasehi
  false,                          //tampilanHijriah
  false,                          //tampilanPasaranJawa
  false,                          //tampilanTanggalMinang
  true,                           //tampilanImsak
  false,                          //tampilanTerbit
  false,                          //tampilanDuha
  false,                          //tampilanTerbenam

  5,                              //kecepatanEfek

  { {0, 0},                       // padam 1
    {0, 0},                       // padam 2
    {0, 0},                       // padam 3
    {0, 0},                       // padam 4
    {0, 0}                        // padam 5
  },

  { {0, 0},                       // nyala 1
    {0, 0},                       // nyala 2
    {0, 0},                       // nyala 3
    {0, 0},                       // nyala 4
    {0, 0}                        // nyala 5
  },


  {false, true, false, false, true, true, false, true, true}, //aktifAlarm[jumlahWaktuShalat];
  {false, false, false, false, false},  //aktifAlarmIqamah[5];
  false,                           //simbolHP
  false,                          //simbolKakiBersentuhan;
  false,                          //simbolDilarangBerisik;
  {false, false, false, false, false}, //aktifTartil[5];
  {false, false, false, false, false}, //aktifTarhim[5];
  true,                           //aktifAlarmWaktuMasuk

  {0, 0, 0, 0, 0, 0, 0, 0, 0},    //jarakAlarm[jumlahWaktuShalat];
  5,                              //jumlahAlarm
  5,                              //jumlahAlarmIqamah
  5,                              //lamaAzan

  {20, 15, 15, 15, 15},           //lamaTartil
  {1, 2, 3, 4, 5},                //suaraTartil
  {5, 5, 5, 5, 5},                //lamaTarhim
  {6, 6, 6, 6, 6},                //suaraTarhim
  {0, 5, 0, 5, 5},                //lamaIqamah
  {10, 20, 20, 15, 20},           //lamaShalat
  45,                             //lamaShalatJumat
  5,                              //lamaSimbolGambar;

  {false, false, false, false, false}, //weekerAktif[5];
  { {0, 0},                       // weeker 1
    {0, 0},                       // weeker 2
    {0, 0},                       // weeker 3
    {0, 0},                       // weeker 4
    {0, 0}                        // weeker 5
  },

  0,                              //fontPesanTartil
  0,                              //fontPesanTarhim
  0,                              //fontPesanSetelahAzan
  0,                              //fontPesanSelamaIqamah
  0,                              //fontPesanSebelumShalat
  0,                              //fontPesanInformasiTetap
  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, //fontInformasi

  5,                              //kecepatanPesanTartil
  5,                              //kecepatanPesanTarhim
  5,                              //kecepatanPesanSetelahAzan
  5,                              //kecepatanPesanSelamaIqamah
  5,                              //kecepatanPesanSebelumShalat
  5,                              //kecepatanInformasiTetap
  {5, 5, 5, 5, 5, 5, 5, 5, 5, 5}, //kecepatanInformasi

  false,                          //aktifPesanTartil
  false,                          //aktifPesanTarhim
  false,                          //aktifPesanSetelahAzan
  false,                          //aktifPesanSelamaIqamah
  false,                          //aktifPesanSebelumShalat
  false,                          //aktifPesanInformasiTetap
  {false, false, false, false, false, false, false, false, false, false},//aktifInformasi
  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, //sambungInformasi

  { {0, 0},                       //jadwalManual[jumlahWaktuShalat];
    {0, 0},
    {0, 0},
    {0, 0},
    {0, 0},
    {0, 0},
    {0, 0},
    {0, 0},
    {0, 0},

  },
  { {0, 0},                       //jadwalOnline[jumlahWaktuShalat];
    {0, 0},
    {0, 0},
    {0, 0},
    {0, 0},
    {0, 0},
    {0, 0},
    {0, 0},
    {0, 0},

  },

  0,                              //modeJadwal
  {
    0,                            // zonaWaktu
    Shafii,                       // metodeAsar
    10,                           // aturImsak
    2,                            // aturSubuh
    2,                            // aturTerbit
    2,                            // aturDuha
    2,                            // aturZuhur
    2,                            // aturAsar
    2,                            // aturTerbenam
    2,                            // aturMagrib
    2,                            // aturIsya

    //Padang
    -0.9797,                      // lintang
    100.4047,                     // bujur
    0,                            // ketinggian
    20.0,                         // sudutSubuh
    0.833,                        // sudutTerbit
    4.5,                          // sudutDuha
    0.833,                        // sudutMagrib
    18.0,                         // sudutIsya
  },

  0,                              //alamatPesanTartil
  0,                              //alamatPesanTarhim
  0,                              //alamatPesanSetelahAzan
  0,                              //alamatPesanSelamaIqamah
  0,                              //alamatPesanSebelumShalat
  0,                              //alamatPesanInformasiTetap
  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, //alamatInformasi

};



void pengaturanAwal()
{
  uint16_t ptr = &settingAwal;
  for (uint16_t i = 0; i < sizeof(Setting); i++)
  {
    byte dataDefault = pgm_read_byte(ptr++);
    EEPROM.update(i, dataDefault);
  }

  Wire.beginTransmission(I2CEEPROM_ADDRESS);
  Wire.write((uint8_t)0);
  Wire.write((uint8_t)0);
  Wire.write((uint8_t)0);
  Wire.endTransmission();
}


#endif
