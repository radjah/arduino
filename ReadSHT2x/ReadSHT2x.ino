/****************************************************************
 * ReadSHT2x
 *  An example sketch that reads the sensor and prints the
 *  relative humidity to the PC's serial port
 *
 *  Tested with:
 *    - SHT21-Breakout Humidity sensor from Modern Device
 *    - SHT2x-Breakout Humidity sensor from MisensO Electronics
 ***************************************************************/

#include <Wire.h>
#include <SHT2x.h>

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


void setup()
{
  Wire.begin();
  lcd.begin(20, 4);
  lcd.home();
  lcd.print("T('C):");
  lcd.setCursor(0, 1);
  lcd.print("H (%):");
  lcd.setCursor(0, 2);
//  char privet[8]={0xCF,0xF0,0xE8,0xE2,0xE5,0xF2,0x21,0x00};
//  lcd.print(privet);
  Serial.begin(9600);
}

void loop()
{
  char prnt[15] = "";
  lcd.setCursor(6, 0);
  dtostrf(SHT2x.GetTemperature(), 14, 2, prnt);
  lcd.print(prnt);
  lcd.setCursor(6, 1);
  dtostrf(SHT2x.GetHumidity(), 14, 2, prnt);
  lcd.print(prnt);
  delay(1000);
}

