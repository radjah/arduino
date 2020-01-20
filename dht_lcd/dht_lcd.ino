#include <DHT.h>
#include <Adafruit_BMP085.h>
#include <Wire.h>
#include <LCD.h>
#include <LiquidCrystal_I2C.h>

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

// Датчики
Adafruit_BMP085         bmp;
DHT                     dht(8, DHT22);

void setup() {
  lcd.begin(16, 2);
  lcd.setBacklightPin(BACKLIGHT, POSITIVE);
  lcd.setBacklight(HIGH);
  lcd.clear();
  lcd.home();
  // "Запуск" DHT11
  dht.begin();
  // Шаблон для дисплея
  lcd.setCursor(0, 0);
  lcd.print("T('C) ");
  lcd.setCursor(0, 1);
  lcd.print("Hum(%)");
}

void loop() {
  float flt;
  char Str[10] = "";
  // Данные с BMP180
  flt = dht.readTemperature();
  // Вывод температуры
  lcd.setCursor(6, 0);
  dtostrf(flt, 10, 2, Str);
  lcd.print(Str);
  // Данные с DHT11
  flt = dht.readHumidity();
  // Вывод влажности
  dtostrf(flt, 10, 2, Str);
  lcd.setCursor(6, 1);
  lcd.print(Str);
  // Задержка
  delay(2000);
}
