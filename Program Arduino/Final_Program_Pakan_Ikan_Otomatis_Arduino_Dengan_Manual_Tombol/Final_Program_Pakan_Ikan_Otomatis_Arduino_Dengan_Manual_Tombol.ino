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

//PUSH BUTTON
#define BUTTON_PIN 5
bool buttonState = false;
bool lastButtonState = false;

//DISPLAY MODE
unsigned long previousMillis = 0;
const long interval = 3000; // 3 detik per tampilan
int displayMode = 0; // 0=tanggal, 1=jadwal1, 2=jadwal2, 3=jadwal3

//JADWAL PUKAN (mudah diubah)
const int JAM_PAGI = 6;
const int MENIT_PAGI = 0;
const int JAM_SIANG = 12;
const int MENIT_SIANG = 0;
const int JAM_SORE = 18;
const int MENIT_SORE = 0;
const int JUMLAH_PAKAN = 15;
const String LABEL_PAGI = "Pagi";
const String LABEL_SIANG = "Siang";
const String LABEL_SORE = "Sore";

void setup() {
  Serial.begin(9600);

  //RTC
  if (! rtc.begin()) {
    Serial.println("Couldn't find RTC");
    Serial.flush();
    while (1) delay(10);
  }
  //  rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  // rtc.adjust(DateTime(2014, 1, 21, 3, 0, 0));

  //LCD
  //  lcd.begin();
  // kalo error ubah lcd.begin(); jadi koding dibawah ini
  lcd.init();
  lcd.backlight();

  //SERVO
  mekanik.attach(4);
  mekanik.write(0);

  //PUSH BUTTON
  pinMode(BUTTON_PIN, INPUT_PULLUP);
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

  //LCD DISPLAY ROTATION
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;
    displayMode++;
    if (displayMode > 3) displayMode = 0;
    lcd.clear(); // Clear screen when switching
  }

  // Baris kedua selalu sama (jam, menit, detik, suhu)
  lcd.setCursor(0, 1);
  String waktu = "";
  if (jam < 10) waktu += "0";
  waktu += String(jam) + ":";
  if (menit < 10) waktu += "0";
  waktu += String(menit) + ":";
  if (detik < 10) waktu += "0";
  waktu += String(detik) + " ";
  waktu += String(suhu, 1) + "C";
  lcd.print(waktu);

  // Baris pertama bergantian (dengan padding untuk clear sisa karakter)
  lcd.setCursor(0, 0);
  String baris1 = "";

  if (displayMode == 0) {
    // Tampilkan tanggal
    baris1 = hari.substring(0, 3) + ",";
    if (tanggal < 10) baris1 += "0";
    baris1 += String(tanggal) + "/";
    if (bulan < 10) baris1 += "0";
    baris1 += String(bulan) + "/" + String(tahun);
  }
  else if (displayMode == 1) {
    // Tampilkan jadwal pagi
    baris1 = LABEL_PAGI + ": ";
    if (JAM_PAGI < 10) baris1 += "0";
    baris1 += String(JAM_PAGI) + ":";
    if (MENIT_PAGI < 10) baris1 += "0";
    baris1 += String(MENIT_PAGI);
  }
  else if (displayMode == 2) {
    // Tampilkan jadwal siang
    baris1 = LABEL_SIANG + ": ";
    if (JAM_SIANG < 10) baris1 += "0";
    baris1 += String(JAM_SIANG) + ":";
    if (MENIT_SIANG < 10) baris1 += "0";
    baris1 += String(MENIT_SIANG);
  }
  else if (displayMode == 3) {
    // Tampilkan jadwal sore
    baris1 = LABEL_SORE + ": ";
    if (JAM_SORE < 10) baris1 += "0";
    baris1 += String(JAM_SORE) + ":";
    if (MENIT_SORE < 10) baris1 += "0";
    baris1 += String(MENIT_SORE);
  }

  // Padding dengan spasi sampai 16 karakter
  while (baris1.length() < 16) {
    baris1 += " ";
  }
  lcd.print(baris1);

  //PUSH BUTTON CHECK
  buttonState = digitalRead(BUTTON_PIN);
  if (buttonState == LOW && lastButtonState == HIGH) {
    // Button pressed (dengan debouncing)
    delay(50); // debounce delay
    if (digitalRead(BUTTON_PIN) == LOW) {
      Serial.println("Button pressed - Kasih pakan manual");
      kasih_pakan(JUMLAH_PAKAN);
    }
  }
  lastButtonState = buttonState;

  //KONDISI OTOMATIS
  if ( (jam == JAM_PAGI) && (menit == MENIT_PAGI) && (detik == 1) ) {
    kasih_pakan(JUMLAH_PAKAN);
  }
  if ( (jam == JAM_SIANG) && (menit == MENIT_SIANG) && (detik == 1) ) {
    kasih_pakan(JUMLAH_PAKAN);
  }
  if ( (jam == JAM_SORE) && (menit == MENIT_SORE) && (detik == 1) ) {
    kasih_pakan(JUMLAH_PAKAN);
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
