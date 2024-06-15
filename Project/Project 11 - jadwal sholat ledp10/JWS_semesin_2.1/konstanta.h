#ifndef KONSTANTA_H
#define KONSTANTA_H

enum indekInformasi
{
  informasiPembuka,
  informasiWaktu,
  informasiTanggalMasehi1,
  informasiTanggalMasehi2,
  informasiImsak,
  informasiSubuh,
  informasiTerbit,
  informasiDuha,
  informasiZuhur,
  informasiAsar,
  informasiTerbenam,
  informasiMagrib,
  informasiIsya,
  informasiTetap,
  informasiText,
  jumlahInformasi,
};


enum NamaHari
{
  Minggu,
  Senin,
  Selasa,
  Rabu,
  Kamis,
  Jumat,
  Sabtu
};

enum _modeOperasi
{
  modeInformasi,
  modeShalat,
  modePadam,
};



enum PerintahBluetooth
{
  tidakAda,//0
  inisialisasi,//1
  aksesMasuk,//2
  aksesKeluar,//3
  gantiKataKunci,//4
  simpanDataString,//5
  masukModeEntri,//6
  keluarModeEntri,//7
  ambilData,//8
  simpanData,//9
  perintah,//10
  cekAkses,//11
};

enum parameterSimpanData {
  simpanPengaturanJadwal = 1,
  simpanTampilanJadwal,
  simpanTampilanShalat,
  simpanTampilanInformasi,
  simpanPengaturanUmum,
  simpanPengaturanWaktu,
};

enum KodePerintah
{
  perintahOffset0,
  perintahNyala,
  perintahPadam,
  perintahSelesaiTarhim,
  perintahSelesaiAzan,
  perintahSelesaiIqamah,
  perintahSelesaiShalat,
  perintahKecerahan,
  perintahDemo,
  perintahSettingAwal,
  perintahJumlahPanel,
  perintahMainkanMusik,
  perintahMusikBerhenti,
  perintahSetWaktu,
  perintahMulaiTarhim,
  perintahMulaiAzan,
  perintahMulaiIqamah,
  perintahMulaiShalat,
  perintahMulaiTartil,
  perintahSelesaiTartil,
};

enum NamaShalat {
  Imsak,
  Subuh,
  Terbit,
  Duha,
  Zuhur,
  Asar,
  Terbenam,
  Magrib,
  Isya,
  jumlahWaktuShalat,
};

enum rumahIbadah {
  Masjid = 1,
  Mushalla,
  Langgar,
  Surau,
  Rumah,
};

const char strWaktuIbadah[][9] PROGMEM = {//Berdasarkan KBBI
  "Imsak",
  "Subuh",
  "Terbit",
  "Duha",
  "Zuhur",
  "Asar",
  "Terbenam",
  "Magrib",
  "Isya",
  "Jumat",
};
#define indexJumat 9
int8_t konversiShalatWajib[] = { -1, 0, -1, -1, 1, 2, -1, 3, 4, 9};

const char strShalat[] PROGMEM = "Shalat";
const char strPengingat[] PROGMEM = "Alarm";
const char strAzan[] PROGMEM = "Azan";
const char strIqamah[] PROGMEM = "Iqamah";
const char strJumat[] PROGMEM = "Jumat";
const char strTarhim[] PROGMEM = "Tarhim";
const char strTartil[] PROGMEM = "Tartil";
const char strJadwalShalat[] PROGMEM = "Jadwal Shalat";

const char strNamaBulan3[][4] PROGMEM = {
  "Jan",
  "Feb",
  "Mar",
  "Apr",
  "Mei",
  "Jun",
  "Jul",
  "Agt",
  "Sep",
  "Okt",
  "Nov",
  "Des",
};
const char strNamaHariMasehi[][7] PROGMEM = {
  "Minggu",
  "Senin",
  "Selasa",
  "Rabu",
  "Kamis",
  "Jum'at",
  "Sabtu",
};
const char strNamaBulanMasehi[][10] PROGMEM = {
  "Januari",
  "Februari",
  "Maret",
  "April",
  "Mei",
  "Juni",
  "Juli",
  "Agustus",
  "September",
  "Oktober",
  "November",
  "Desember",
};
const char strNamaHariHijriah[][15] PROGMEM = {
  "al-Ahad",
  "al-Itsnayn",
  "ats-Tsalaatsa'",
  "al-Arba'aa",
  "al-Khamsatun",
  "al-Jumu'ah",
  "as-Sabat",
};

const char strNamaBulanHijriah[][14] PROGMEM = {
  "Muharram",
  "Shafar",
  "Rabiul Awal",
  "Rabiul Akhir",
  "Jumadil Awal",
  "Jumadil Akhir",
  "Rajab",
  "Sya'ban",
  "Ramadhan",
  "Syawal",
  "Dzulqa'dah",
  "Dzulhijah"
};

const char strNamaHariPasaran[][7] PROGMEM = {
  "Legi",
  "Pahing",
  "Pon",
  "Wage",
  "Kliwon",
};

const char strNamaBulanJawa[][12] PROGMEM = {
  "Sura",
  "Sapar",
  "Mulud",
  "Bakda Mulud",
  "Jumadilawal",
  "Jumadilakir",
  "Rejeb",
  "Ruwah",
  "Pasa",
  "Sawal",
  "Sela",
  "Besar",
};

const char strNamaHariMinang[][8] PROGMEM = {
  "Akaik",
  "Sinayan",
  "Salasa",
  "Raba'a",
  "Kamih",
  "Jumaik",
  "Sabtu",
};

const char strNamaBulanPariaman[][15] PROGMEM = {
  "Tabuik",
  "Sapa",
  "Muluik",
  "Adiak Muluik",
  "Adiak Muluik 2",
  "Carai",
  "Sambareh",
  "Lamang",
  "Puaso",
  "Rayo",
  "Adiak Rayo",
  "Haji"
};

#endif
