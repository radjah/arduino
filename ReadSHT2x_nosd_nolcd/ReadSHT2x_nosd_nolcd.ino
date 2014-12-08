#include <OneWire.h>
#include <DallasTemperature.h>
#include <Wire.h>
#include <RTClib.h>
#include <SHT2x.h>
#include <Adafruit_BMP085.h>
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

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

// Часы DS1307
RTC_DS1307 rtc;

// Температуры на улице
OneWire dswire(2);
DallasTemperature sensors(&dswire);
DeviceAddress dsaddr;

// Давление и температура внутри
Adafruit_BMP085 bmp;

void setup()
{
  Serial.begin(9600);
  // i2c
  Wire.begin();
  // Часы
  rtc.begin();
  // Dallas
  sensors.getAddress(dsaddr, 0);
  sensors.setResolution(dsaddr, 12);
  // BMP
  bmp.begin();
  // Радио
  radio.begin();
  // Настройка
  radio.setRetries(15, 15);
  radio.setAutoAck(1);
  radio.setChannel(50);
  radio.setDataRate(RF24_250KBPS);
  // Пайпы на запись и чтение
  radio.openWritingPipe(pipes[0]);
  radio.openReadingPipe(1, pipes[1]);
  radio.startListening();
}

void loop()
{
  // Пакет для отправки
  sendtemp st;
  // Получаем время
  DateTime now = rtc.now();
  // GMT + 3
  DateTime MSK (now + 3 * 3600);
  st.dt = MSK.unixtime();
  // Запрос измерения
  sensors.requestTemperatures();
  st.intemp = bmp.readTemperature();
  st.outtemp = sensors.getTempC(dsaddr);
  st.hum = SHT2x.GetHumidity();
  st.pres = bmp.readPressure() / 133.32;
  // Отправляем
  radio.stopListening();
  bool ok = radio.write(&st, sizeof(sendtemp));
  if (ok)
    Serial.println("Sended!");
  else
    Serial.println("ERROR: not sended!");
  radio.startListening();
  // Задержка
  delay(1000);
}

