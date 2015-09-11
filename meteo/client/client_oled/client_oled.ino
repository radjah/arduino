#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <nRF24L01.h>
#include <RF24.h>
#include "printf.h"
#include <RTClib.h>

#define OLED_RESET 4
#define CSNPIN 53
#define CEPIN 49

struct sendtemp {
  float outtemp;
  float intemp;
  float pres;
  float hum;
};

// Радио
RF24 radio(CEPIN, CSNPIN);
// Дисплей
Adafruit_SSD1306 display(OLED_RESET);
// Часы DS1307
RTC_DS1307 rtc;

// Пайпы
const uint64_t pipes[2] = { 0xF0F0F0F0E1LL, 0xF0F0F0F0D2LL };

void setup() {
  Wire.begin();
  display.begin();
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0, 0);
  display.print("Waiting...");
  display.display();
  Serial.begin(9600);
  printf_begin();
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
  rtc.begin();
  Serial.println("This is CLIENT");
  Serial.println("Waiting...");
}

void loop() {
  if ( radio.available() )
  {
    bool done = false;
    sendtemp st;
    while (!done)
    {
      done = radio.read( &st, sizeof(sendtemp) );
      display.clearDisplay();
      display.setTextSize(1);
      display.setCursor(0, 16);
      display.print("Tout('C):");
      display.setCursor(0, 24);
      display.print("Tin ('C):");
      display.setCursor(0, 32);
      display.print("Pin (mm):");
      display.setCursor(0, 40);
      display.print("Hum  (%):");
      DateTime now = rtc.now();
      DateTime MSK (now + 4 * 3600);
      char dt[18] = "";
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
      display.setCursor(0, 0);
      display.print(dt);
      display.setCursor(54, 16);
      char tempcstr[12] = "";
      dtostrf(st.outtemp, 11, 2, tempcstr);
      display.print(tempcstr);
      display.setCursor(54, 24);
      dtostrf(st.intemp, 11, 2, tempcstr);
      display.print(tempcstr);
      display.setCursor(54, 32);
      dtostrf(st.pres, 11, 2, tempcstr);
      display.print(tempcstr);
      display.setCursor(54, 40);
      dtostrf(st.hum, 11, 2, tempcstr);
      display.print(tempcstr);
      display.display();
      delay(20);
    };
  };
  delay(100);
}

