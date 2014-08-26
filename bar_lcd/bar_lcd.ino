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

LiquidCrystal_I2C       lcd(LCD_I2C_ADDR,LCD_EN,LCD_RW,LCD_RS,LCD_D4,LCD_D5,LCD_D6,LCD_D7);

// Датчики
Adafruit_BMP085         bmp;
DHT                     dht(2, DHT11); // S подключен к D2

void setup() {
  Serial.begin(9600,SERIAL_8N2);
  lcd.begin(20,4);
  lcd.setBacklightPin(BACKLIGHT,POSITIVE);
  lcd.setBacklight(HIGH);
  lcd.clear();
  lcd.home();
  // Проверка на доступность датчика
  if (!bmp.begin()) {
	lcd.print("BMP180 ERROR!");
	while (1) {}
  }
  // "Запуск" DHT11
  dht.begin();
  // Шаблон для дисплея
  lcd.setCursor(0,0);
  lcd.print("T('C) ");
  lcd.setCursor(0,1);
  lcd.print("P(Hg) ");
  lcd.setCursor(0,2);
  lcd.print("Hum(%)");
}

void loop() {
  float Temp, Hum;          // Температура и влажность
  int32_t pres;             // Давление
  char StrHum[15]="";       // Влажность строкой
  char StrPresHg[15]="";    // Давление в мм рт.ст. строкой
  char StrTemp[15]="";      // Температура строкой
  // Данные с BMP180
  Temp=bmp.readTemperature();
  pres=bmp.readPressure();
  // Вывод температуры
  lcd.setCursor(6, 0);
  dtostrf(Temp, 14, 2, StrTemp);
  lcd.print(StrTemp);
  // Вывод давления
  float preshg=float(pres)/133.32;
  dtostrf(preshg, 14, 2, StrPresHg);
  lcd.setCursor(6, 1);
  lcd.print(StrPresHg);
  // Данные с DHT11
  Hum=dht.readHumidity();
  // Вывод влажности
  dtostrf(Hum, 14, 2, StrHum);
  lcd.setCursor(6, 2);
  lcd.print(StrHum);
  Serial.print("T");
  Serial.print(Temp);
  Serial.print("P");
  Serial.print(preshg);
  Serial.print("H");
  Serial.print(Hum);
  Serial.print(char(13));
  // Задержка
  delay(2000);
}
