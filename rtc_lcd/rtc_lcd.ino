#include <Wire.h>
#include <RTClib.h>
#include <LCD.h>
#include <LiquidCrystal_I2C.h>

RTC_DS1307 rtc;

// Порты LCD
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

void setup () {
  Wire.begin();
  rtc.begin();
  lcd.begin(20, 4);
  lcd.setBacklightPin(BACKLIGHT, POSITIVE);
  lcd.setBacklight(HIGH);
  lcd.clear();
  lcd.home();
}

void loop () {
    DateTime now = rtc.now();
    DateTime MSK (now.unixtime() + 4 * 3600);
    char dt[21] = "";
    sprintf(dt, "%04d/%02d/%02d  %02d:%02d:%02d", MSK.year(),
                                                  MSK.month(),
                                                  MSK.day(),
                                                  MSK.hour(),
                                                  MSK.minute(),
                                                  MSK.second());
    lcd.setCursor(0, 0);
    lcd.print(dt);
    delay(1000);
}
