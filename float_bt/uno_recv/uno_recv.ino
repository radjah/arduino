#include <Wire.h>
#include <LCD.h>
#include <LiquidCrystal_I2C.h>
#include <SoftwareSerial.h>
#include <CyberLib.h>

#define LCD_I2C_ADDR    0x27 // Define I2C Address where the PCF8574T is
#define BACKLIGHT     3
#define LCD_EN  2
#define LCD_RW  1
#define LCD_RS  0
#define LCD_D4  4
#define LCD_D5  5
#define LCD_D6  6
#define LCD_D7  7

LiquidCrystal_I2C       lcd(LCD_I2C_ADDR, LCD_EN, LCD_RW, LCD_RS, LCD_D4, LCD_D5, LCD_D6, LCD_D7);
SoftwareSerial swSerial(10, 11);

void setup() {
  swSerial.begin(9600);
  lcd.begin(20, 4);
  lcd.setBacklightPin(BACKLIGHT, POSITIVE);
  lcd.setBacklight(HIGH);
  lcd.clear();
  lcd.home();
}

void loop() {
  if (swSerial.available() > 0) {
    char str[21] = "";
    float temp = swSerial.parseFloat();
    dtostrf(temp, 20, 2, str);
    lcd.setCursor(0, 0);
    lcd.print(str);
    temp = swSerial.parseFloat();
    dtostrf(temp, 20, 2, str);
    lcd.setCursor(0, 1);
    lcd.print(str);
  }
//  delay_ms(100);
}
