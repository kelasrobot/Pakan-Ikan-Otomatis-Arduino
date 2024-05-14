#include <Servo.h>
Servo mekanik;

void setup() {
  mekanik.attach(4);
  mekanik.write(0);
}

void loop() {
  delay(20000);
  mekanik.write(150);
  delay(100);
  mekanik.write(0);
  delay(100);
  mekanik.write(150);
  delay(100);
  mekanik.write(0);
  delay(100);
}
