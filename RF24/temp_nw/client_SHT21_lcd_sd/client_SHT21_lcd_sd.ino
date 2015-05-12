// Arduini Mega as CLIENT

#include <CyberLib.h>
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include "printf.h"
#include <Wire.h>
#include <LCD.h>
#include <LiquidCrystal_I2C.h>
#include <RTClib.h>
#include <SD.h>

// Порты LCD
#define LCD1_I2C_ADDR    0x27
#define LCD2_I2C_ADDR    0x23
#define BACKLIGHT     3
#define LCD_EN  2
#define LCD_RW  1
#define LCD_RS  0
#define LCD_D4  4
#define LCD_D5  5
#define LCD_D6  6
#define LCD_D7  7

LiquidCrystal_I2C      lcd1(LCD1_I2C_ADDR, LCD_EN, LCD_RW, LCD_RS, LCD_D4, LCD_D5, LCD_D6, LCD_D7);
LiquidCrystal_I2C      lcd2(LCD2_I2C_ADDR, LCD_EN, LCD_RW, LCD_RS, LCD_D4, LCD_D5, LCD_D6, LCD_D7);

// Пины радио
#if  defined (__AVR_ATmega2560__) || defined (__AVR_ATmega2561__)
#define CSNPIN 49
#define CEPIN 48
#else
#define CSNPIN 10
#define CEPIN 9
#endif


struct sendtemp {
  uint32_t dt;       // Время unixtime
  float    outtemp;  // Температура на улице
  float    intemp;   // Температура внутри
  float    pres;     // Давление
  float    hum;      // Влажность относительная
};


// Карта памяти
//File logfile;        // Файл лога
bool isSD = true;    // Флаг присутствия карты

#define SDCS 4      // CS SD-карты


// Радио
RF24 radio(CEPIN, CSNPIN);

// Пайпы
const uint64_t pipes[2] = { 0xF0F0F0F0E1LL, 0xF0F0F0F0D2LL };

void setup() {
  Serial.begin(9600);
  // Настройка выводов
  D4_Out;
  D10_Out;
  D49_Out;
  D53_Out;
  // Отключить Ethernet
  D10_High;
  // Выключить RF24
  D49_High;
  // Включить SD-карту
  D4_Low;
  // Включаем дисплей
  lcd1.begin(20, 4);
  lcd1.setBacklightPin(BACKLIGHT, POSITIVE);
  lcd1.setBacklight(HIGH);
  lcd1.clear();
  lcd1.home();
  lcd1.setCursor(0, 1);
  lcd1.print("T('C):");
  lcd1.setCursor(0, 2);
  lcd1.print("H (%):");
  lcd1.setCursor(0, 3);
  lcd1.print("P(Hg):");
  lcd2.begin(20, 4);
  lcd2.setBacklightPin(BACKLIGHT, POSITIVE);
  lcd2.setBacklight(HIGH);
  lcd2.clear();
  lcd2.home();
  lcd2.setCursor(0, 0);
  lcd2.print("SD:--");
  lcd2.setCursor(8, 0);
  lcd2.print("L Wr:--");
  lcd2.setCursor(0, 1);
  lcd2.print("Log:------------");
  printf_begin();
  // Включаем карту
  // Если адаптер завелся
  if (!SD.begin(SDCS)) {
    isSD = false;
    Serial.println("SD not found!");
    lcd2.setCursor(3, 0);
    lcd2.print("ERR");
  } else {
    isSD = true;
    Serial.println("SD found!");
    lcd2.setCursor(3, 0);
    lcd2.print("OK ");
  }
  Serial.println(isSD);
  // Выключить SD-карту
  D4_High;
  // Включить RF24
  D49_Low;
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
  // Работает только RF24
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
      // Вывод в порт
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
      lcd1.setCursor(0, 0);
      lcd1.print(dt);
      // Под температуру
      char tmpc[7] = "";
      // Под всё остальное
      char prnt[15] = "";
      // Вывод температуры внутри и снаружи.
      lcd1.setCursor(7, 1);
      dtostrf(st.intemp, 6, 2, tmpc);
      lcd1.print(tmpc);
      lcd1.print("/");
      if (st.outtemp == -127.0) {
        lcd1.print("ERROR!");
      } else {
        dtostrf(st.outtemp, 6, 2, tmpc);
        lcd1.print(tmpc);
      }
      // Вывод относительной влажности
      lcd1.setCursor(6, 2);
      dtostrf(st.hum, 14, 2, prnt);
      lcd1.print(prnt);
      // Вывод давления
      lcd1.setCursor(6, 3);
      dtostrf(st.pres, 14, 2, prnt);
      lcd1.print(prnt);
      // Запись на карту памяти
      if (isSD == true) {
        Serial.println("Writing...");
        // Выключить RF24
        D49_High;
        // Включить SD-карту
        D4_Low;
        // Файл для записи
        char fn[13] = "";
        sprintf(fn, "%04d%02d%02d.txt", MSK.year(),
                MSK.month(),
                MSK.day());
        lcd2.setCursor(4, 1);
        lcd2.print(fn);
        File logfile = SD.open(fn, FILE_WRITE);
        Serial.println(logfile);
        if (logfile == true) {
          logfile.print(dt);
          logfile.print(" ");
          logfile.print(st.outtemp);
          logfile.print(" ");
          logfile.print(st.intemp);
          logfile.print(" ");
          logfile.print(st.pres);
          logfile.print(" ");
          logfile.println(st.hum);
          logfile.close();
          // print to the serial port too:
          Serial.println("Log OK!");
          lcd2.setCursor(13, 0);
          lcd2.print("OK ");
        } else {
          lcd2.setCursor(13, 0);
          lcd2.print("ERR");
        }
        // Выключить SD-карту
        D4_High;
        // Включить RF24
        D49_Low;
      }
    }
    delay(20);
  };
};

