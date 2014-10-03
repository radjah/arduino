#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include "printf.h"
#include <Wire.h>
#include <LCD.h>
#include <LiquidCrystal_I2C.h>
#include <RTClib.h>

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


#define CSNPIN 10
#define CEPIN 9

struct sendtemp {
  float outtemp;
  float intemp;
  float pres;
  float hum;
};

// Радио
RF24 radio(CEPIN, CSNPIN);

// Часы DS1307
RTC_DS1307 rtc;

// Пайпы
const uint64_t pipes[2] = { 0xF0F0F0F0E1LL, 0xF0F0F0F0D2LL };

void setup() {
  Serial.begin(9600);
  printf_begin();
  // Включаем дисплей
  lcd.begin(20, 4);
  lcd.setBacklightPin(BACKLIGHT, POSITIVE);
  lcd.setBacklight(HIGH);
  lcd.clear();
  lcd.home();
  lcd.setCursor(0, 1);
  lcd.print("To/i('C):");
  lcd.setCursor(0, 2);
  lcd.print("Pin (mm):");
  lcd.setCursor(0, 3);
  lcd.print("Hum  (%):");
  // Включаем радио
  radio.begin();
  // Настройка
  radio.setRetries(15, 15);
  radio.setAutoAck(1);
  radio.setChannel(50);
  radio.setDataRate(RF24_250KBPS);
  // Пайпы на запись и чтение
  radio.openWritingPipe(pipes[1]);
  radio.openReadingPipe(1, pipes[0]);
  radio.startListening();
  radio.printDetails();
  Wire.begin();
  rtc.begin();
  Serial.println("This is CLIENT");
}

void loop() {
  if ( radio.available() )
  {
    bool done = false;
    sendtemp st;
    while (!done)
    {
      done = radio.read( &st, sizeof(sendtemp) );
      DateTime now = rtc.now();
      DateTime MSK (now + 4 * 3600);
      char dt[18] = "";
      sprintf(dt, "%02d/%02d/%02d %02d:%02d:%02d", MSK.year(),
                                                   MSK.month(),
                                                   MSK.day(),
                                                   MSK.hour(),
                                                   MSK.minute(),
                                                   MSK.second());
      Serial.println("Recived!");
      Serial.print("Time: ");
      Serial.println(dt);
      Serial.print("Outtemp: ");
      Serial.println(st.outtemp);
      Serial.print("Intemp: ");
      Serial.println(st.intemp);
      Serial.print("Inpres: ");
      Serial.println(st.pres);
      Serial.print("Inhum: ");
      Serial.println(st.hum);
      // Вывод на дисплей
      lcd.setCursor(0, 0);
      lcd.print(dt);
      char tempcstr[12] = "";
      char tempstr[6] = "";
      lcd.setCursor(9, 1);
      dtostrf(st.outtemp, 5, 2, tempstr);
      lcd.print(tempstr);
      lcd.print("/");
      dtostrf(st.intemp, 5, 2, tempstr);
      lcd.print(tempstr);
      lcd.setCursor(9, 2);
      dtostrf(st.pres, 11, 2, tempcstr);
      lcd.print(tempcstr);
      lcd.setCursor(9, 3);
      dtostrf(st.hum, 11, 2, tempcstr);
      lcd.print(tempcstr);
      delay(20);
    };
  };
  delay(100);
}
