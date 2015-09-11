#include <CyberLib.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include <Wire.h>
#include <RTClib.h>
#include <SHT2x.h>
#include <Adafruit_BMP085.h>
#include <LCD.h>
#include <LiquidCrystal_I2C.h>
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <SD.h>

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

#define CSNPIN 10
#define CEPIN 9

#define CARDPIN 8

File logfile;        // Файл лога
bool isSD = true;    // Флаг присутствия карты

struct sendtemp {
  uint32_t dt;       // Время unixtime
  float    outtemp;  // Температура на улице
  float    intemp;   // Температура внутри
  float    pres;     // Давление
  float    hum;      // Влажность относительная
};

LiquidCrystal_I2C       lcd(LCD_I2C_ADDR, LCD_EN, LCD_RW, LCD_RS, LCD_D4, LCD_D5, LCD_D6, LCD_D7);

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
  digitalWrite(CARDPIN, HIGH);
  // Настройка
  radio.setRetries(15, 15);
  radio.setAutoAck(1);
  radio.setChannel(50);
  radio.setDataRate(RF24_250KBPS);
  // Пайпы на запись и чтение
  radio.openWritingPipe(pipes[0]);
  radio.openReadingPipe(1, pipes[1]);
  radio.startListening();
  // Подключение карты
  if (!SD.begin(CARDPIN)) {
    isSD = false;
    Serial.println("SD not found!");
  }
  // Шаблон на дисплее
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
  //  char privet[8]={0xCF,0xF0,0xE8,0xE2,0xE5,0xF2,0x21,0x00};
  //  lcd.print(privet);
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
  char dt[21] = "";
  sprintf(dt, "%04d/%02d/%02d  %02d:%02d:%02d", MSK.year(),
          MSK.month(),
          MSK.day(),
          MSK.hour(),
          MSK.minute(),
          MSK.second());
  // Если карта подключена, то открываем файл
  lcd.setCursor(0, 0);
  lcd.print(dt);
  // Под температуру
  char tmpc[7] = "";
  // Под всё остальное
  char prnt[15] = "";
  // Запрос измерения
  sensors.requestTemperatures();
  // Вывод температуры внутри и снаружи.
  lcd.setCursor(7, 1);
  st.intemp = bmp.readTemperature();
  dtostrf(st.intemp, 6, 2, tmpc);
  lcd.print(tmpc);
  lcd.print("/");
  st.outtemp = sensors.getTempC(dsaddr);
  dtostrf(st.outtemp, 6, 2, tmpc);
  lcd.print(tmpc);
  // Вывод относительной влажности
  lcd.setCursor(6, 2);
  st.hum = SHT2x.GetHumidity();
  dtostrf(st.hum, 14, 2, prnt);
  lcd.print(prnt);
  // Вывод давления
  lcd.setCursor(6, 3);
  st.pres = bmp.readPressure() / 133.32;
  dtostrf(st.pres, 14, 2, prnt);
  lcd.print(prnt);
  if (isSD) {
    digitalWrite(CSNPIN, HIGH);
    Serial.print("Writing...");
    char filename[13] = "";
    char mestime[10] = "";
    sprintf(filename, "%04d%02d%02d.txt", MSK.year(), MSK.month(), MSK.day());
    sprintf(mestime, "%02d:%02d:%02d", MSK.hour(), MSK.minute(), MSK.second());
    logfile = SD.open(filename, FILE_WRITE);
    logfile.print(mestime);
    logfile.print(" ");
    logfile.print(st.outtemp);
    logfile.print(" ");
    logfile.print(st.intemp);
    logfile.print(" ");
    logfile.print(st.pres);
    logfile.print(" ");
    logfile.println(st.hum);
    logfile.close();
    digitalWrite(CARDPIN, HIGH);
    Serial.println("OK!");
  }
  // Отправляем
  radio.stopListening();
  bool ok = radio.write(&st, sizeof(sendtemp));
  if (ok)
    Serial.println("Sended!");
  else
    Serial.println("ERROR: not sended!");
  radio.startListening();
  // Задержка
  delay_ms(1000);
}

