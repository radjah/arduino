#include <CyberLib.h>
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

volatile int count = 0;

void setup() {
  pinMode(3, INPUT);
  digitalWrite(3, LOW);
  D13_In;
  D13_Low;
  attachInterrupt(1, D2Int, RISING);
  lcd.begin(20,4);
  lcd.setBacklightPin(BACKLIGHT,POSITIVE);
  lcd.setBacklight(HIGH);
  lcd.clear();
  lcd.home();
}

void loop() {
  lcd.clear();
  lcd.print(count);
  delay_ms(1000);
}
void D2Int(){
  count++;
}
