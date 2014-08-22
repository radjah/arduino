#include <Adafruit_BMP085.h>

#include <Wire.h>
#include <LCD.h>
#include <LiquidCrystal_I2C.h>

// Init LCD
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

Adafruit_BMP085 bmp;

void setup() {
  lcd.begin(20,4);
  lcd.setBacklightPin(BACKLIGHT,POSITIVE);
  lcd.setBacklight(HIGH);
  lcd.clear();
  lcd.home();
  // put your setup code here, to run once:
  if (!bmp.begin()) {
	lcd.print("BMP180 ERROR!");
	while (1) {}
  }
  lcd.setCursor(0,0);
  lcd.print("T('C) ");
  lcd.setCursor(0,1);
  lcd.print("P(Pa) ");
  lcd.setCursor(0,2);
  lcd.print("P(Hg) ");
}

void loop() {
  float Temp;
  int32_t pres;
  char StrPres[15]="";
  char StrPresHg[15]="";
  char StrTemp[15]="";
  Temp=bmp.readTemperature();
  pres=bmp.readPressure();
  lcd.setCursor(6, 0);
  dtostrf(Temp, 14, 2, StrTemp);
  lcd.print(StrTemp);
  sprintf(StrPres, "%14lu", pres);
  lcd.setCursor(6, 1);
  lcd.print(StrPres);
  dtostrf(float(pres)/133.32, 14, 2, StrPresHg);
  lcd.setCursor(6, 2);
  lcd.print(StrPresHg);
  delay(2000);
}
