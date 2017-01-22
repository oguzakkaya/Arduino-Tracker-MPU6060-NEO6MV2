#include <TinyGPS++.h> //gps kütüphane
#include <SoftwareSerial.h>
#include "I2Cdev.h"
#include "MPU6050.h" //gyro kütüphane
#include "Wire.h"
static void printDateTime(TinyGPSDate &d, TinyGPSTime &t); //prototip printDateTime fonksiyon
static void printFloat(float val, bool valid, int len, int prec); //prototip printFloat fonksiyon
static void printInt(unsigned long val, bool valid, int len); //prototip printInt fonksiyon
static void smartDelay(unsigned long ms); //prototip smartDelay fonksiyon

MPU6050 accelgyro; // Mpu6050 sensör tanımlama
int16_t ax, ay, az; //ivme tanımlama
int16_t gx, gy, gz; //gyro tanımlama
static const int RXPin = 4, TXPin = 3;
static const uint32_t GPSBaud = 9600; //gpsbaud değeri atama

TinyGPSPlus gps; //gps tanımlama

SoftwareSerial ss(RXPin, TXPin); //gps seri bağlantı

void setup()
{
  Wire.begin();
  Serial.begin(115200);
  Serial.println("I2C cihazlar baslatiliyor...");
  Serial.println("GPS baslatiliyor...");
  accelgyro.initialize();
  Serial.println("Test cihazi baglantilari...");
  Serial.println(accelgyro.testConnection() ? "MPU6050 baglanti basarili." : "MPU6050 baglantisi basarisiz.");
  Serial.println(ss.read() ? "GPS baglantisi basarili." : "GPS baglantisi basarisiz.");
  Serial.println();
  ss.begin(GPSBaud);
  Serial.println(F("Enlem      Boylam      Tarih      Zaman      Yukseklik  MF Uzaklik       X       Y       Z"));
  Serial.println(F("---------------------------------------------------------------------------------------------"));
}

void loop()
{
  static const double DUMF_LAT = 40.904310, DUMF_LON = 31.180550; //mühendislik fakültesi konumu
  accelgyro.getMotion6(&ax, &ay, &az, &gx, &gy, &gz); //gyrodan veri alma
  printFloat(gps.location.lat(), gps.location.isValid(), 11, 6); //enlem yazdırma
  printFloat(gps.location.lng(), gps.location.isValid(), 12, 6); //boylam yazdırma
  printDateTime(gps.date, gps.time);  //tarih saat yazdırma
  Serial.print("  ");
  printFloat(gps.altitude.meters(), gps.altitude.isValid(), 7, 2); //yukseklik yazdırma
  Serial.print("\t");
  unsigned long distanceKmToDuzceMF =
    (unsigned long)TinyGPSPlus::distanceBetween(
      gps.location.lat(),
      gps.location.lng(),
      DUMF_LAT,
      DUMF_LON) / 1000;
  printInt(distanceKmToDuzceMF, gps.location.isValid(), 9); //Düzce Mühendislik Fakültesine olan Uzaklık
  Serial.print("\t"); Serial.print(ax); Serial.print("\t"); //gyro x ekseni yazdırma
  Serial.print(ay); Serial.print("\t"); //gyro y ekseni yazdırma
  Serial.print(az); //gyro z ekseni yazdırma
  Serial.println();
  smartDelay(1000);

  if (millis() > 5000 && gps.charsProcessed() < 10)
    Serial.println(F("GPS verisi alinamadi. Lutfen kablolamayi kontrol edin."));
}

static void smartDelay(unsigned long ms) //gecikme fonksiyonu
{
  unsigned long start = millis();
  do
  {
    while (ss.available())
      gps.encode(ss.read());
  } while (millis() - start < ms);
}

static void printFloat(float val, bool valid, int len, int prec) //yazdirma fonksiyonu
{
  if (!valid)
  {
    while (len-- > 1)
      Serial.print('*');
    Serial.print(' ');
  }
  else
  {
    Serial.print(val, prec);
    int vi = abs((int)val);
    int flen = prec + (val < 0.0 ? 2 : 1); // . and -
    flen += vi >= 1000 ? 4 : vi >= 100 ? 3 : vi >= 10 ? 2 : 1;
    for (int i = flen; i < len; ++i)
      Serial.print(' ');
  }
  smartDelay(0);
}

static void printInt(unsigned long val, bool valid, int len)
{
  char sz[32] = "*****************";
  if (valid)
    sprintf(sz, "%ld", val);
  sz[len] = 0;
  for (int i = strlen(sz); i < len; ++i)
    sz[i] = ' ';
  if (len > 0)
    sz[len - 1] = ' ';
  Serial.print(sz);
  smartDelay(0);
}

static void printDateTime(TinyGPSDate &d, TinyGPSTime &t) //tarih, zaman bastirma fonksiyonu
{
  if (!d.isValid())
  {
    Serial.print(F("********** "));
  }
  else
  {
    char sz[32];
    sprintf(sz, "%02d/%02d/%02d ", d.month(), d.day(), d.year());
    Serial.print(sz);
  }

  if (!t.isValid())
  {
    Serial.print(F("******** "));
  }
  else
  {
    char sz[32];
    sprintf(sz, "%02d:%02d:%02d ", t.hour() + 3, t.minute(), t.second());
    Serial.print(sz);
  }
  smartDelay(0);
}
