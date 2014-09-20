#include <CyberLib.h>

#include <Wire.h>
#include <Adafruit_BMP085.h>
#include <SoftwareSerial.h>

SoftwareSerial swSerial(10, 11);
Adafruit_BMP085 bmp;


void setup() {
  swSerial.begin(9600);
  bmp.begin();
}

void loop() {
  float temp = bmp.readTemperature();
  float pres = bmp.readPressure() / 133.32;
  swSerial.println(temp);
  swSerial.println(pres);
  delay_ms(500);
}
