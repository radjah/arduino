#include <OneWire.h>
#include <DallasTemperature.h>
#include <Wire.h>
#include <RTClib.h>
#include <SHT2x.h>
#include <Adafruit_BMP085.h>
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <DHT.h>
#include <EEPROM2.h>
#include <CyberLib.h>

// Датчик висит на D6
#define DHTPIN 6
#define DHTTYPE DHT22

#define TIMESPAN 3
//#define SETTIME

#define CSNPIN 10
#define CEPIN 9

struct sendtemp {
  uint32_t dt;       // Время unixtime
  float    outtemp;  // Температура на улице
  float    intemp;   // Температура внутри
  float    pres;     // Давление
  float    humin;    // Влажность относительная
  float    humout;   // Влажность относительная
};

// Радио
RF24 radio(CEPIN, CSNPIN);

// Влажность на улице
DHT dht(DHTPIN, DHTTYPE);

// Пайпы
const uint64_t pipes[2] = { 0xF0F0F0F0E1LL, 0xF0F0F0F0D2LL };

// Часы DS1307
RTC_DS1307 rtc;

// Температуры на улице
OneWire dswire(2);
DallasTemperature sensors(&dswire);
DeviceAddress dsaddr1;
DeviceAddress dsaddr2;

bool UseBMP, CanServ;

// Давление и температура внутри
Adafruit_BMP085 bmp;

void setup()
{
  D7_Out;
  D8_Out;
  Serial.begin(9600);
  // i2c
  Wire.begin();
  // Часы
  rtc.begin();
  // DS
  sensors.begin();
#ifdef SETTIME
  // 15 - примерное время прошивки в секундах
  rtc.adjust(DateTime(__DATE__, __TIME__) - TimeSpan(TIMESPAN * 3600 - 15));
#endif
  // Проверяем датчики
  byte dc = sensors.getDeviceCount();
  // Если датчиков нет или их много, то уходим в бесконечный цикл
  switch (dc) {
    case 0:
      Serial.println(F("DS18B20 not found!"));
      Blink(250);
      break;
    case 1:
      Serial.println(F("Found one DS18B20!"));
      Serial.println(F("Using BMP180 for inner temperature."));
      sensors.getAddress(dsaddr1, 0);
      UseBMP = true;
      CanServ = true;
      break;
    default:
      Serial.println(F("Found two or more DS18B20!"));
      Serial.println(F("Reading ADDR from EEPROM"));
      ReadAddr(dsaddr1, 0);
      ReadAddr(dsaddr2, 1);
      Serial.print(F("dsaddr1 outtemp: "));
      printAddress(dsaddr1);
      Serial.println();
      Serial.print(F("dsaddr2  intemp: "));
      printAddress(dsaddr2);
      Serial.println();
      sensors.setResolution(dsaddr2, 10);
      UseBMP = false;
  }
  ServiceMode();
  sensors.setResolution(dsaddr1, 10);
  // BMP
  bmp.begin();
  // DHT
  dht.begin();
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
  if (UseBMP) {
    st.intemp = bmp.readTemperature();
  } else {
    st.intemp = sensors.getTempC(dsaddr2);
  }
  st.outtemp = sensors.getTempC(dsaddr1);
  st.humin = SHT2x.GetHumidity();
  st.pres = bmp.readPressure() / 133.32;
  st.humout = dht.readHumidity();
  // Отправляем
  radio.stopListening();
  bool ok = radio.write(&st, sizeof(sendtemp));
  if (ok)
    Serial.println(F("Sended!"));
  else
    Serial.println(F("ERROR: not sended!"));
  radio.startListening();
  // Задержка
  delay_ms(1500);
}

// Настройка датчиков

// Мигаем об ошибке
void Blink (int period) {
  while (1) {
    D13_Out;
    D13_High;
    delay_ms(period);
    D13_Low;
    delay_ms(period);
  }
}

// Сервисный режим с записью адресов в EEPROM
void ServiceMode() {
  // Признаки записи;
  D7_In;
  D8_In;
  sensors.getAddress(dsaddr1, 0);
  if (D7_Read == true) {
    Serial.print(F("Writing addr for outtemp: "));
    printAddress(dsaddr1);
    Serial.println();
    WriteAddr(dsaddr1, 0);
    Serial.println(F("Done! Remove jumper and cycle power"));
    Blink(1000);
  } else {
    // Если писать адрес второго
    if (D8_Read == true) {
      Serial.print(F("Writing addr for inttemp: "));
      printAddress(dsaddr1);
      Serial.println();
      WriteAddr(dsaddr1, 1);
      Serial.println(F("Done! Remove jumper and cycle power"));
      Blink(1000);
    }
  }
}
// Запись адреса в EEPROM
void WriteAddr (DeviceAddress DSAddr, const int DSNum) {
  EEPROM_write_mem(DSNum * 8, DSAddr, 8);
}

// Запись адреса в EEPROM
void ReadAddr (DeviceAddress DSAddr, const int DSNum) {
  EEPROM_read_mem(DSNum * 8, DSAddr, 8);
}

// Вывод адреса в Serial
void printAddress(DeviceAddress deviceAddress)
{
  for (uint8_t i = 0; i < 8; i++)
  {
    if (deviceAddress[i] < 16) Serial.print("0");
    Serial.print(deviceAddress[i], HEX);
  }
}

