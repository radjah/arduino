#include <Adafruit_BMP085.h>
#include <SoftwareSerial.h>
#include <Wire.h>
#include <LCD.h>
#include <LiquidCrystal_I2C.h>

// Init LCD
#define LCD_I2C_ADDR  0x27
#define BACKLIGHT     3
#define LCD_EN        2
#define LCD_RW        1
#define LCD_RS        0
#define LCD_D4        4
#define LCD_D5        5
#define LCD_D6        6
#define LCD_D7        7

Adafruit_BMP085 bmp;

// Объект для работы с дисплеем
LiquidCrystal_I2C lcd(LCD_I2C_ADDR,
                      LCD_EN,
                      LCD_RW,
                      LCD_RS,
                      LCD_D4,
                      LCD_D5,
                      LCD_D6,
                      LCD_D7);

// Программный Serial
SoftwareSerial swSerial(10, 11); // RX - 10, TX - 11

// Инициализая
void setup() {
  // Датчик давления
  bmp.begin();
  // ЖК дисплей
  lcd.begin(20, 4);
  lcd.setBacklightPin(BACKLIGHT, POSITIVE);
  lcd.setBacklight(HIGH);
  lcd.clear();
  //  lcd.home();
  // Шаблон показаний
  lcd.setCursor(0, 0);
  lcd.print("T1:_____C T2:_____C");
  lcd.setCursor(0, 1);
  lcd.print("T3:_____C T4:_____C");
  lcd.setCursor(0, 2);
  lcd.print("T5:_____C");
  lcd.setCursor(0, 3);
  lcd.print("T:_____'C P:_____mm");
  // Последовательный порт
  Serial.begin(9600, SERIAL_8N2);
  Serial1.begin(9600, SERIAL_8N2);
  Serial2.begin(9600, SERIAL_8N2);
  Serial3.begin(9600, SERIAL_8N2);
  swSerial.begin(9600);
}

void loop() {
  // Курсор на первый символ
  lcd.home();
  // Чтение из портов
  ReadFromSerial(&Serial, 0, 3);
  ReadFromSerial(&Serial1, 0, 13);
  ReadFromSerial(&Serial2, 1, 3);
  ReadFromSerial(&Serial3, 1, 13);
  ReadFromSerial(&swSerial, 2, 3);
  // Температору и давление
  float readval;            // Параметр числом
  char sreadval[6] = "";    // Параметр строкой
  // Чтение и вывод температуры
  readval = bmp.readTemperature();
  dtostrf(readval, 5, 1, sreadval);
  lcd.setCursor(2, 3);
  lcd.print(sreadval);
  // Чтение и вывод давления
  readval = bmp.readPressure();
  dtostrf(readval / 133.32, 5, 1, sreadval);
  lcd.setCursor(12, 3);
  lcd.print(sreadval);
  // Задержка между замерами
  delay(100);
}

// Вывод на экран
void PrintResult(String Input, int lcdLine, int lcdPos) {
  float DigTmp;          // Температура числом
  char  TmpOut[6] = "";  // Температура строкой
  String  StrOut = "";   // Подстрока с температурой
  Input.trim();
  /*
    // Выделяем e авось пригодится
    String Eps = "";
    Eps = Input.substring(5, 7);
  */
  // Выделяем и выводим температуру
  StrOut = Input.substring(7, 11);
  // Курсор на позицию
  lcd.setCursor(lcdPos, lcdLine);
  switch (StrOut[0]) {
    case '-':
      // Отрицательные значения
      if (StrOut[1] == 'F') {
        StrOut[1] = '0';
      }
      DigTmp = float(StrOut.toInt()) / 10;
      dtostrf(DigTmp, 5, 1, TmpOut);
      lcd.print(TmpOut);
      break;
    case '>':
      // Превышение диапазона
      lcd.print(">>>>>");
      break;
    case '<':
      // Ниже диапазона
      lcd.print("<<<<<");
      break;
    default:
      // Положительные в диапазоне
      lcd.print(" ");
      lcd.print(StrOut);
  }
}

// Чтение строки из порта, обработка, проверка и вывод на экран
void ReadFromSerial(Stream *p_Serial, int lcdLine, int lcdPos) {
  // Переменные
  int i;                 // Счетчик
  char RecvChar;         // Полученный байт
  // Serial
  String RecvStr = "";   // Полученная строка
  String StrForCh = "=";
  // Чтение из порта Serial0
  if (p_Serial->available() > 0) {
    RecvChar = p_Serial->read();
    // Проверка на начало последовательности
    if (RecvChar == '=') {
      // Вся ли строка в буфере?
      while (p_Serial->available() < 14) {
        delay(10);
      }
      if (p_Serial->available() >= 14) {
        // Если вся, то читаем всю.
        for (i = 0; i < 14; i++) {
          RecvStr += char(p_Serial->read());
        }
        StrForCh += RecvStr;
        // Проверка результата и вывод
        if (ChechCRC(StrForCh)) {
          PrintResult(RecvStr, lcdLine, lcdPos);
        }
      }
    }
  }
}

// Проверка контрольной суммы
boolean ChechCRC (String RecvStr) {
  char sum = 0;
  // Сложить все тетрады...
  for (int i = 0; i < 12; i++) {
    sum ^= (((RecvStr[i] >> 4) ^ RecvStr[i]) & B00001111);
  }
  // ... и преобразовать в '0'..'F'
  char calc = (sum < 10) ? ('0' + sum) : ('A' + sum - 10);
  return (RecvStr[12] == calc) ? true : false;
}

