//RTC
#include "RTClib.h"
RTC_DS3231 rtc;
char daysOfTheWeek[7][12] = {"Ahad", "Senin", "Selasa", "Rabu", "Kamis", "Jum'at", "Sabtu"};
int jam, menit, detik;
int tanggal, bulan, tahun;
String hari;
float suhu;

//LCD
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27, 16, 2);

//SERVO
#include <Servo.h>
Servo mekanik;

void setup() {
  Serial.begin(9600);

  //RTC
  if (! rtc.begin()) {
    Serial.println("Couldn't find RTC");
    Serial.flush();
    while (1) delay(10);
  }
  rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  // rtc.adjust(DateTime(2014, 1, 21, 3, 0, 0));

  //LCD
  //  lcd.begin();
  // kalo error ubah lcd.begin(); jadi koding dibawah ini
  lcd.init();
  lcd.backlight();

  //SERVO
  mekanik.attach(4);
  mekanik.write(0);
}

void loop() {

  //RTC
  DateTime now = rtc.now();
  jam     = now.hour();
  menit   = now.minute();
  detik   = now.second();
  tanggal = now.day();
  bulan   = now.month();
  tahun   = now.year();
  hari    = daysOfTheWeek[now.dayOfTheWeek()];
  suhu    = rtc.getTemperature();
  Serial.println(String() + hari + ", " + tanggal + "-" + bulan + "-" + tahun);
  Serial.println(String() + jam + ":" + menit + ":" + detik);
  Serial.println();

  //LCD
  lcd.setCursor(0, 0);
  lcd.print(String() + hari + "," + tanggal + "-" + bulan + "-" + tahun);
  lcd.print(" ");
  lcd.setCursor(0, 1);
  lcd.print(String() + jam + ":" + menit + ":" + detik);
  lcd.print("  ");
  lcd.print(suhu);
  lcd.print(" ");

  //KONDISI
  if ( (jam == 7) && (menit == 30) && (detik == 1) ) {
    kasih_pakan(15);
  }
  if ( (jam == 18) && (menit == 15) && (detik == 1) ) {
    kasih_pakan(15);
  }

}

void kasih_pakan(int jumlah) {
  for (int i = 1; i <= jumlah; i++) {
    mekanik.write(150);
    delay(100);
    mekanik.write(0);
    delay(100);
  }
}
