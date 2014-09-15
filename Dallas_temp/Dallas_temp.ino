#include <CyberLib.h>
#include <DallasTemperature.h>
#include <OneWire.h>
#include <Wire.h>
#include <LCD.h>
#include <LiquidCrystal_I2C.h>
#include <Adafruit_BMP085.h>

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


OneWire dswire(2);
DallasTemperature sensors(&dswire);
DeviceAddress dsaddr;
Adafruit_BMP085         bmp;

void setup() {
  bmp.begin();
  lcd.begin(20,4);
  lcd.setBacklightPin(BACKLIGHT,POSITIVE);
  lcd.setBacklight(HIGH);
  lcd.clear();
  lcd.home();
  sensors.getAddress(dsaddr, 0);
  lcd.print("Dev  :");
  printAddress(dsaddr);
  lcd.setCursor(0,1);
  lcd.print("T('C):");
  sensors.setResolution(dsaddr, 12);
  sensors.requestTemperatures();
  lcd.setCursor(0,2);
  lcd.print("T('C):");
}

void loop() {
  char tempcstr[15]="";
  float tempC = 0.0;
  tempC = sensors.getTempC(dsaddr);
  lcd.setCursor(6,1);
  dtostrf(tempC, 14, 2, tempcstr);
  lcd.print(tempcstr);
  tempC = bmp.readTemperature();
  lcd.setCursor(6,2);
  dtostrf(tempC, 14, 2, tempcstr);
  lcd.print(tempcstr);
  sensors.requestTemperatures();
  delay(100);
}

void printAddress(DeviceAddress deviceAddress)
{
  for (uint8_t i = 0; i < 8; i++)
  {
    if (deviceAddress[i] < 16) Serial.print("0");
    lcd.print(deviceAddress[i], HEX);
  }
}
