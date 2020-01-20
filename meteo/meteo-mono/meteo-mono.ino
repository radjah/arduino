#include <OneWire.h>
#include <DallasTemperature.h>
#include <Wire.h>
#include <LCD.h>
#include <LiquidCrystal_I2C.h>
#include <RTClib.h>
#include <SHT2x.h>
#include <Adafruit_BMP085.h>
#include <SPI.h>
#include <DHT.h>
#include <EEPROM2.h>
#include <CyberLib.h>
#include <SdFat.h>

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


// Датчик висит на D6
#define DHTPIN 4
#define DHTTYPE DHT21

#define TIMESPAN 3
// #define SETTIME

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

// Карта памяти

SdFat SD;

//File logfile;        // Файл лога
bool isSD = false;     // Флаг присутствия карты

#define SDCS 10      // CS SD-карты

// Влажность на улице
DHT dht(DHTPIN, DHTTYPE);

// Часы DS1307
RTC_DS1307 rtc;

// Температуры на улице
OneWire dswire(6);
DallasTemperature sensors(&dswire);
DeviceAddress dsaddr1, dsaddr2;

bool UseBMP, CanServ;

// Давление и температура внутри
Adafruit_BMP085 bmp;

void setup()
{
  pinMode(7, INPUT_PULLUP);
  pinMode(8, INPUT_PULLUP);;
  Serial.begin(115200);
  // i2c
  Wire.begin();
  // Часы
  rtc.begin();
  // DS
  sensors.begin();
#ifdef SETTIME
  // 15 - примерное время прошивки в секундах
  rtc.adjust(DateTime(__DATE__, __TIME__) - TimeSpan(TIMESPAN * 3600 - 10));
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
      sensors.setResolution(dsaddr1, 12);
      UseBMP = true;
      CanServ = true;
      break;
    default:
      Serial.println(F("Found two or more DS18B20!"));
      Serial.println(F("Reading ADDR from EEPROM"));
      ReadAddr(dsaddr1, 0); // outtemp
      ReadAddr(dsaddr2, 1); // intemp
      Serial.print(F("dsaddr1 outtemp: "));
      printAddress(dsaddr1);
      Serial.println();
      Serial.print(F("dsaddr2  intemp: "));
      printAddress(dsaddr2);
      Serial.println();
      sensors.setResolution(dsaddr2, 12);
      sensors.setResolution(dsaddr1, 12);
      UseBMP = false;
      CanServ = false;
  }
  ServiceMode();
  // BMP
  Serial.println(F("BMP180 init"));
  bmp.begin();
  Serial.println(F("BMP180 OK"));
  // DHT
  dht.begin();
  Serial.println(F("DHT21 OK"));
  // LCD
  lcd1.begin(20, 4);
  lcd1.setBacklightPin(BACKLIGHT, POSITIVE);
  lcd1.setBacklight(HIGH);
  lcd1.clear();
  lcd1.home();
  lcd1.setCursor(0, 1);
  lcd1.print(F("T('C):"));
  lcd1.setCursor(0, 2);
  lcd1.print(F("H (%):"));
  lcd1.setCursor(0, 3);
  lcd1.print(F("P(Hg):"));
  lcd2.begin(20, 4);
  lcd2.setBacklightPin(BACKLIGHT, POSITIVE);
  lcd2.setBacklight(HIGH);
  lcd2.clear();
  lcd2.home();
  lcd2.setCursor(0, 0);
  lcd2.print(F("SD:--"));
  lcd2.setCursor(10, 0);
  lcd2.print(F("Wr:--"));
  lcd2.setCursor(0, 1);
  lcd2.print(F("Log:------------"));
  // All green!
  Serial.println(F("Setup finished"));
}

void loop()
{
  // Пакет для отправки
  sendtemp st;
  // Запрос измерения
  sensors.requestTemperatures();
  // Получаем время
  DateTime now = rtc.now();
  // GMT + 3
  DateTime MSK (now + 3 * 3600);
  // TODO: сделать что-нибудь с этим огрызком
  st.dt = MSK.unixtime();
  char dt[21] = "";
  sprintf(dt, "%04d/%02d/%02d  %02d:%02d:%02d", MSK.year(),
          MSK.month(),
          MSK.day(),
          MSK.hour(),
          MSK.minute(),
          MSK.second());
  if (UseBMP == true) {
    st.intemp = bmp.readTemperature();
  } else {
    st.intemp = sensors.getTempC(dsaddr2);
  }
  st.outtemp = sensors.getTempC(dsaddr1);
  st.humin = SHT2x.GetHumidity();
  st.pres = bmp.readPressure() / 133.32;
  st.humout = dht.readHumidity();
  Serial.println(st.dt);
  Serial.println(st.intemp);
  Serial.println(st.outtemp);
  Serial.println(st.humin);
  Serial.println(st.humout);
  Serial.println(st.pres);
  // Вывод на дисплей
  lcd1.setCursor(0, 0);
  lcd1.print(dt);
  // Под температуру
  char tmpc[7] = "";
  // Под всё остальное
  char prnt[15] = "";
  // Вывод температуры внутри и снаружи.
  lcd1.setCursor(7, 1);
  if (st.intemp == -127.0) {
    lcd1.print(F("ERROR!"));
  } else {
    dtostrf(st.intemp, 6, 2, tmpc);
    lcd1.print(tmpc);
  }
  lcd1.print("/");
  if (st.outtemp == -127.0) {
    lcd1.print(F("ERROR!"));
  } else {
    dtostrf(st.outtemp, 6, 2, tmpc);
    lcd1.print(tmpc);
  }
  // Вывод относительной влажности
  lcd1.setCursor(7, 2);
  dtostrf(st.humin, 6, 2, tmpc);
  lcd1.print(tmpc);
  lcd1.print("/");
  dtostrf(st.humout, 6, 2, tmpc);
  lcd1.print(tmpc);
  // Вывод давления
  lcd1.setCursor(6, 3);
  dtostrf(st.pres, 14, 2, prnt);
  lcd1.print(prnt);
  // Запись на карту памяти
  if (isSD == true) {
    Serial.print(F("Writing..."));
    // Файл для записи
    char fn[13] = "";
    sprintf(fn, "%04d%02d%02d.txt", MSK.year(),
            MSK.month(),
            MSK.day());
    lcd2.setCursor(4, 1);
    lcd2.print(fn);
    File logfile = SD.open(fn, FILE_WRITE);
    if (logfile == true) {
      // Валидация полученных данных
      if (!(st.intemp == 85.0) & !(st.intemp == -127) & !(st.outtemp == 85.0) & !(st.outtemp == -127) & !(isnan(st.humout))) {
        logfile.print(dt);
        logfile.print(" ");
        logfile.print(st.outtemp);
        logfile.print(" ");
        logfile.print(st.intemp);
        logfile.print(" ");
        logfile.print(st.pres);
        logfile.print(" ");
        logfile.print(st.humin);
        logfile.print(" ");
        logfile.println(st.humout);
        logfile.close();
        // print to the serial port too:
        Serial.println(F("OK!"));
        lcd2.setCursor(13, 0);
        lcd2.print(F("OK "));
        } else {
        Serial.println(F("Bad data!"));
        lcd2.setCursor(13, 0);
        lcd2.print(F("Bad"));
        logfile.close();
      };
    } else {
      // При ошибке записи запускаем отсчет
      Serial.println(F("ERR!"));
      lcd2.setCursor(13, 0);
      lcd2.print(F("ERR"));
      isSD = false;
    }
  }
  // Задержка
  delay_ms(1000);
  CheckSD();
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
  if (!D7_Read or !D8_Read) {
    sensors.getAddress(dsaddr1, 0);
  }
  if (D7_Read == false) { // outtemp
    Serial.print(F("Writing addr for outtemp: "));
    printAddress(dsaddr1);
    Serial.println();
    WriteAddr(dsaddr1, 0);
    Serial.println(F("Done! Remove jumper and cycle power"));
    Blink(1000);
  } else {
    // Если писать адрес второго
    if (D8_Read == false) { // intemp
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

// Проверка карты памяти
void CheckSD() {
  // Попытки
  static byte tr = 0;
  unsigned long curt = millis();
  static unsigned long oldt = 0;
  // Позиция вывода
  lcd2.setCursor(3, 0);
  // Если ошибка карты
  if (isSD == false) {
    // Проверка интервала
    if (curt - oldt > 1000) {
      oldt = curt;
      // Если счетчик досчитал
      if (tr < 1) {
        lcd2.print(F("Init "));
        lcd2.setCursor(3, 0);
        if (!SD.begin(SDCS)) {
          // SD-карта не включилась. Ошибка.
          isSD = false;
          Serial.println(F("SD not found!"));
          lcd2.print(F("ERR  "));
          tr = 5;
        } else {
          // SD-карта на местое. Работаем.
          isSD = true;
          Serial.println(F("SD found!"));
          lcd2.print(F("OK   "));
        }
      } else {
        // Вывод ошибки и счетчика
        lcd2.print(F("ERR "));
        lcd2.print(tr);
        tr--;
      }
    }
  }
}

