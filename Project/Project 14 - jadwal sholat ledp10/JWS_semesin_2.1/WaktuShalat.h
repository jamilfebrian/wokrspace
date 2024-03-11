#ifndef WAKTUShalat_SEMESIN_H
#define WAKTUShalat_SEMESIN_H

#include "Arduino.h"
#include "trigonometri.h"
#include "setting.h"
#include "konstanta.h"
#include "fungsi.h"

#define IterasiKomputasiJadwalShalat      5
#define offsetPengaturanJadwal            offsetof(Setting, dataPengaturan)
#define offsetJadwal                      offsetof(Setting, dataPengaturan) + offsetof(DataPengaturan, jadwalMatahari)

//struct Jadwal
//{
//  JamDanMenit shalat[jumlahWaktuShalat];
//};

double julian_date;
double sun_declination;
double equation_of_time;
double lintang;
double bujur;

double fix_angle(double a)
{
  a = a - 360.0 * floor(a / 360.0);
  a = a < 0.0 ? a + 360.0 : a;
  return a;
}

double fix_hour(double a)
{
  a = a - 24.0 * floor(a / 24.0);
  a = a < 0.0 ? a + 24.0 : a;
  return a;
}

double time_diff(double time1, double time2)
{
  return fix_hour(time2 - time1);
}
double night_portion(double angle)
{
  return 1.0 * angle / 60.0;
}
void get_float_time_parts(double time, int& hours, int& minutes)
{
  time = fix_hour(time + 0.5 / 60);		// add 0.5 minutes to round
  hours = floor(time);
  minutes = floor((time - hours) * 60);
}

void getSunEquation(double jd)
{
  double d = jd - 2451545.0;
  double g = fix_angle(357.529 + 0.98560028 * d);
  double q = fix_angle(280.459 + 0.98564736 * d);
  double la = fix_angle(q + 1.915 * dsin(g) + 0.020 * dsin(2 * g));

  double e = 23.439 - 0.00000036 * d;

  sun_declination = darcsin(dsin(e) * dsin(la));
  double ra = darctan2(dcos(e) * dsin(la), dcos(la)) / 15.0;
  ra = fix_hour(ra);
  equation_of_time = q / 15.0 - ra;
}
double compute_mid_day(double time)
{
  getSunEquation(julian_date + time);
  double z = fix_hour(12 - equation_of_time);
  return z;
}
double compute_time(double angle, double time)
{
  double z = compute_mid_day(time);
  getSunEquation(julian_date + time);

  double v = 1.0 / 15.0 * darccos((-dsin(angle) - dsin(sun_declination) * dsin(lintang)) / (dcos(sun_declination) * dcos(lintang)));
  return z + (angle > 90.0 ? - v :  v);
}
double compute_asr(int step, int8_t time)  // Shafii: step=1, Hanafi: step=2
{
  getSunEquation(julian_date + time);
  double g = -darccot(step + dtan(fabs(lintang - sun_declination)));
  return compute_time(g , time);
}
void adjust_times(double times[])
{
  for (int i = 0; i < jumlahWaktuShalat; ++i)
    times[i] += 1.0 * (EEPROM.read(offsetPengaturanJadwal + offsetof(DataPengaturan, zonaWaktu)) + 7) - (bujur / 15.0);

  //  adjust_high_lat_times(times);
  double night_time = time_diff(times[Terbenam], times[Terbit]);   // sunset to sunrise

  double sudut;
  EEPROM.get(offsetPengaturanJadwal + offsetof(DataPengaturan, sudutSubuh), sudut);

  // Adjust Fajr
  double fajr_diff = night_portion(sudut) * night_time;
  if (isnan(times[Subuh]) || time_diff(times[Subuh], times[Terbit]) > fajr_diff)
    times[Subuh] = times[Terbit] - fajr_diff;

  EEPROM.get(offsetPengaturanJadwal + offsetof(DataPengaturan, sudutIsya), sudut);

  // Adjust Isha
  double isha_diff = night_portion(sudut) * night_time;
  if (isnan(times[Isya]) || time_diff(times[Terbenam], times[Isya]) > isha_diff)
    times[Isya] = times[Terbenam] + isha_diff;

  EEPROM.get(offsetPengaturanJadwal + offsetof(DataPengaturan, sudutMagrib), sudut);

  // Adjust Magrib
  double magrib_diff = night_portion(sudut) * night_time;
  if (isnan(times[Magrib]) || time_diff(times[Terbenam], times[Magrib]) > magrib_diff)
    times[Magrib] = times[Terbenam] + magrib_diff;

  for (byte i = 0; i < 8; i++)
  {
    times[i + Subuh] += EEPROM.read(offsetPengaturanJadwal + offsetof(DataPengaturan, aturSubuh) + i) / 60.0;
  }
}
void compute_times(double times[])
{
  for (int i = 0; i < jumlahWaktuShalat; ++i)
    times[i] /= 24.0;

  double sudut;

  EEPROM.get(offsetPengaturanJadwal + offsetof(DataPengaturan, sudutSubuh), sudut);
  times[Subuh]    = compute_time(180.0 - sudut, times[Subuh]);
  EEPROM.get(offsetPengaturanJadwal + offsetof(DataPengaturan, sudutTerbit), sudut);
  times[Terbit] = compute_time(180.0 - sudut, times[Terbit]);
  EEPROM.get(offsetPengaturanJadwal + offsetof(DataPengaturan, sudutDuha), sudut);
  times[Duha] = compute_time(180.0 - sudut, times[Duha]);
  times[Zuhur]   = compute_mid_day(times[Zuhur]);
  times[Asar]     = compute_asr(1.0 + EEPROM.read(offsetPengaturanJadwal + offsetof(DataPengaturan, metodeAsar)), times[Asar]);
  EEPROM.get(offsetPengaturanJadwal + offsetof(DataPengaturan, sudutTerbit), sudut);
  times[Terbenam]  = compute_time(sudut, times[Terbenam]);
  EEPROM.get(offsetPengaturanJadwal + offsetof(DataPengaturan, sudutMagrib), sudut);
  times[Magrib] = compute_time(sudut, times[Magrib]);
  EEPROM.get(offsetPengaturanJadwal + offsetof(DataPengaturan, sudutIsya), sudut);
  times[Isya]    = compute_time(sudut, times[Isya]);

}

void compute_day_times(double times[])
{
  byte default_times[] = { 0, 5, 6, 6, 12, 13, 18, 18, 18 };    // default times
  for (byte i = 0; i < jumlahWaktuShalat; ++i)
  {
    times[i] = default_times[i];
  }

  for (int i = 0; i < IterasiKomputasiJadwalShalat; ++i)
    compute_times(times);

  adjust_times(times);
}

void ambilWaktuShalat(Tanggal tanggal)
{
  //			Jadwal jadwal;
  double waktuShalatDouble[jumlahWaktuShalat];

  EEPROM.get(offsetPengaturanJadwal + offsetof(DataPengaturan, lintang), lintang);
  EEPROM.get(offsetPengaturanJadwal + offsetof(DataPengaturan, bujur), bujur);

  julian_date = get_julian_date(tanggal) - bujur / (double) (15 * 24);
  compute_day_times(waktuShalatDouble);

  waktuShalatDouble[Imsak] = waktuShalatDouble[Subuh] - (1.0 * EEPROM.read(offsetPengaturanJadwal + offsetof(DataPengaturan, aturImsak)) / 60.0);
  for (byte i = 0; i < jumlahWaktuShalat; i++)
  {
    int menit;
    int jam;
    get_float_time_parts(waktuShalatDouble[i], jam, menit);
    EEPROM.write(offsetJadwal + (i * 2) + 0, jam);
    EEPROM.write(offsetJadwal + (i * 2) + 1, menit);
    //    jadwal->shalat[i].jam = jam;
    //    jadwal->shalat[i].menit = menit;
  }
  //  return jadwal;
}

#endif
