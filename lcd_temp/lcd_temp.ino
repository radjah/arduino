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

// Объект для работы с дисплеем
LiquidCrystal_I2C lcd(LCD_I2C_ADDR,
                      LCD_EN,
                      LCD_RW,
                      LCD_RS,
                      LCD_D4,
                      LCD_D5,
                      LCD_D6,
                      LCD_D7);

// Инициализая
void setup() {
  // ЖК дисплей
  lcd.begin(20,4);
  lcd.setBacklightPin(BACKLIGHT,POSITIVE);
  lcd.setBacklight(HIGH);
  lcd.clear();
  // Последовательный порт
  Serial.begin(9600);
}

void loop() {
  // Курсор на первый символ
  lcd.home();
  // Переменные
  String RecvStr="";
  String Eps="";
  String Tmp="";
  char sTmp[20]="";
  // Чтение из порта
  if (Serial.available() > 0) {
    char RecvChar = Serial.read();
    // Отладочный вывод
    Serial.println(Serial.available());
    Serial.println(RecvChar);
    // Проверка на начало последовательности
    if (RecvChar == '=') {
      // Вся ли строка в буфере?
      // TODO: исправить на реальную длину без первого символа.
      if (Serial.available() >= 14) {
        // Если вся, то читаем всю.
        for (int i=0;i<14;i++) {
          RecvStr += char(Serial.read());
        }
        // Выводим последовательность
        //lcd.setCursor(0,0);
        RecvStr.trim();
        //lcd.print(RecvStr);
        // Выделяем и выводим e
        lcd.setCursor(0,1);
        Eps=RecvStr.substring(5,7);
        lcd.print(float(Eps.toInt()+1)/100);
        // Выделяем и выводим тепературу
        lcd.setCursor(0,2);
        Tmp=RecvStr.substring(7,11);
        Tmp.trim();
        sprintf(sTmp,"T1=%4d'C T2=TEST'C", Tmp.toInt());
        lcd.print(sTmp);
      }
    }
  }
  delay(1000);
}
