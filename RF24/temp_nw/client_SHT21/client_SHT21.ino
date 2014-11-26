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
  uint32_t dt;       // Время unixtime
  float    outtemp;  // Температура на улице
  float    intemp;   // Температура внутри
  float    pres;     // Давление
  float    hum;      // Влажность относительная
};

// Радио
RF24 radio(CEPIN, CSNPIN);

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
  lcd.print("T('C):");
  lcd.setCursor(0, 2);
  lcd.print("H (%):");
  lcd.setCursor(0, 3);
  lcd.print("P(Hg):");
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
      DateTime MSK (st.dt);
      char dt[21] = "";
      sprintf(dt, "%02d/%02d/%02d  %02d:%02d:%02d", MSK.year(),
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
      // Под температуру
      char tmpc[7] = "";
      // Под всё остальное
      char prnt[15] = "";
      // Вывод температуры внутри и снаружи.
      lcd.setCursor(7, 1);
      dtostrf(st.intemp, 6, 2, tmpc);
      lcd.print(tmpc);
      lcd.print("/");
      dtostrf(st.outtemp, 6, 2, tmpc);
      lcd.print(tmpc);
      // Вывод относительной влажности
      lcd.setCursor(6, 2);
      dtostrf(st.hum, 14, 2, prnt);
      lcd.print(prnt);
      // Вывод давления
      lcd.setCursor(6, 3);
      dtostrf(st.pres, 14, 2, prnt);
      lcd.print(prnt);
      delay(20);
    };
  };
  delay(100);
}
