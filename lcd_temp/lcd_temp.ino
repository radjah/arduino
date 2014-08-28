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
  // Переменные
  int i; // Счетчик
  char RecvChar;
  // Serial
  String RecvStr = "";
  String Eps = "";
  String Tmp = "";
/*  // Serial1
  String s1RecvStr = "";
  String s1Eps = "";
  String s1Tmp = "";
  // Serial2
  String s2RecvStr = "";
  String s2Eps = "";
  String s2Tmp = "";
  // Serial3
  String s3RecvStr = "";
  String s3Eps = "";
  String s3Tmp = "";
  // swSerial
  String swRecvStr = "";
  String swEps = "";
  String swTmp = ""; */
  // Чтение из порта Serial0
  if (Serial.available() > 0) {
    RecvChar = Serial.read();
    // Отладочный вывод
/*    Serial.println(Serial.available());
    Serial.println(RecvChar);*/
    // Проверка на начало последовательности
    if (RecvChar == '=') {
      // Вся ли строка в буфере?
      while (Serial.available() < 14) {
        delay(10);
      }
      if (Serial.available() >= 14) {
        // Если вся, то читаем всю.
        for (i = 0; i < 14; i++) {
          RecvStr += char(Serial.read());
        }
        // Выводим последовательность
        RecvStr.trim();
        // Выделяем и выводим e
        Eps = RecvStr.substring(5, 7);
        // Выделяем и выводим тепературу
        Tmp = RecvStr.substring(7, 11);
        PrintResult(Tmp, 0, 3);
      }
    }
  }
  // Чтение из порта Serial1
  if (Serial1.available() > 0) {
    RecvChar = Serial1.read();
    // Отладочный вывод
/*    Serial.println(Serial1.available());
    Serial.println(RecvChar);*/
    // Проверка на начало последовательности
    if (RecvChar == '=') {
      // Вся ли строка в буфере?
      while (Serial1.available() < 14) {
        delay(10);
      }
      if (Serial1.available() >= 14) {
        // Если вся, то читаем всю.
        for (i = 0; i < 14; i++) {
          RecvStr += char(Serial1.read());
        }
        // Выводим последовательность
        RecvStr.trim();
        // Выделяем и выводим e
        Eps = RecvStr.substring(5, 7);
        // Выделяем и выводим тепературу
        Tmp = RecvStr.substring(7, 11);
        PrintResult(Tmp, 0, 13);
      }
    }
  }
  
  // Чтение из порта Serial2
  if (Serial2.available() > 0) {
    RecvChar = Serial2.read();
    // Отладочный вывод
/*    Serial.println(Serial2.available());
    Serial.println(RecvChar); */
    // Проверка на начало последовательности
    if (RecvChar == '=') {
      // Вся ли строка в буфере?
      while (Serial2.available() < 14) {
        delay(10);
      }
      if (Serial2.available() >= 14) {
        // Если вся, то читаем всю.
        for (i = 0; i < 14; i++) {
          RecvStr += char(Serial2.read());
        }
        // Выводим последовательность
        RecvStr.trim();
        // Выделяем и выводим e
        Eps = RecvStr.substring(5, 7);
        // Выделяем и выводим тепературу
        Tmp = RecvStr.substring(7, 11);
        PrintResult(Tmp, 1, 3);
      }
    }
  }
  // Чтение из порта Serial3
  if (Serial3.available() > 0) {
    RecvChar = Serial3.read();
    // Отладочный вывод
/*    Serial.println(Serial3.available());
    Serial.println(RecvChar); */
    // Проверка на начало последовательности
    if (RecvChar == '=') {
      // Вся ли строка в буфере?
      while (Serial3.available() < 14) {
        delay(10);
      }
      if (Serial3.available() >= 14) {
        // Если вся, то читаем всю.
        for (i = 0; i < 14; i++) {
          RecvStr += char(Serial3.read());
        }
        // Выводим последовательность
        RecvStr.trim();
        // Выделяем и выводим e
        Eps = RecvStr.substring(5, 7);
        // Выделяем и выводим тепературу
        Tmp = RecvStr.substring(7, 11);
        PrintResult(Tmp, 1, 13);
      }
    }
  }
  // Чтение из порта swSeial
  if (swSerial.available() > 0) {
    RecvChar = swSerial.read();
    // Отладочный вывод
    swSerial.println(swSerial.available());
    swSerial.println(RecvChar);
    // Проверка на начало последовательности
    if (RecvChar == '=') {
      // Вся ли строка в буфере?
      while (swSerial.available() < 14) {
        delay(10);
      }
      if (swSerial.available() >= 14) {
        // Если вся, то читаем всю.
        for (i = 0; i < 14; i++) {
          RecvStr += char(swSerial.read());
        }
        // Выводим последовательность
        RecvStr.trim();
        // Выделяем и выводим e
        Eps = RecvStr.substring(5, 7);
        // Выделяем и выводим тепературу
        Tmp = RecvStr.substring(7, 11);
        PrintResult(Tmp, 2, 3);
      }
    }
  }
  float readval; // Температура и давление
  char sreadval[6]="";
  readval = bmp.readTemperature();
  dtostrf(readval, 5, 1, sreadval);
  lcd.setCursor(2, 3);
  lcd.print(sreadval);
  readval = bmp.readPressure();
  dtostrf(readval/133.32, 5, 1, sreadval);
  lcd.setCursor(12, 3);
  lcd.print(sreadval);
  // Задержка между замерами
  delay(100);
}

// Вывод на экран
void PrintResult(String Input, int lcdLine, int lcdPos) {
  float DigTmp;
  char  TmpOut[6] = "";
  swSerial.println(Input);
  swSerial.println(sizeof(Input));
  switch (Input[0]) {
    case '-':
      // Отрицательные значения
      if (Input[1] == 'F') {
        Input[1] = '0';
      }
      DigTmp = float(Input.toInt()) / 10;
      swSerial.println(Input);
      dtostrf(DigTmp, 5, 1, TmpOut);
      swSerial.println(TmpOut);
      lcd.setCursor(lcdPos, lcdLine);
      lcd.print(TmpOut);
      break;
    case '>':
      // Превышение диапазона
      lcd.setCursor(lcdPos, lcdLine);
      lcd.print(">>>>>");
      break;
    case '<':
      // Ниже диапазона
      lcd.setCursor(lcdPos, lcdLine);
      lcd.print("<<<<<");
      break;
    default:
      // Положительные в диапазоне
      lcd.setCursor(lcdPos, lcdLine);
      lcd.print(" ");
      lcd.print(Input);
  }
}
