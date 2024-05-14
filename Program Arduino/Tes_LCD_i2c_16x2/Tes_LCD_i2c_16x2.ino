#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27, 16, 2);

void setup() {
  lcd.begin();

  // kalo error ubah lcd.begin(); jadi koding dibawah ini
  //  lcd.init();
  //  lcd.backlight();
}

void loop() {
  lcd.setCursor(0, 0);
  lcd.print("TEST LCD i2C");
  lcd.setCursor(0, 1);
  lcd.print("KelasRobot.com");
}
