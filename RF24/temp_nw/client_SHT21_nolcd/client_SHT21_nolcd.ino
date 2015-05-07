// Arduini Mega as CLIENT

#include <CyberLib.h>
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include "printf.h"
#include <Wire.h>
#include <RTClib.h>
#include <SD.h>

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
  printf_begin();
  // Включаем карту
  // Если адаптер завелся
  if (!SD.begin(SDCS)) {
    isSD = false;
    Serial.println("SD not found!");
  } else {
    isSD = true;
    Serial.println("SD found!");
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
      char dt[20] = "";
      sprintf(dt, "%04d/%02d/%02d %02d:%02d:%02d", MSK.year(),
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

