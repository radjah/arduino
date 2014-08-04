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

// Массив символов для шкалы
const uint8_t charBitmap[][8] = {
 { 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10 },
 { 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18 },
 { 0x1c, 0x1c, 0x1c, 0x1c, 0x1c, 0x1c, 0x1c, 0x1c },
 { 0x1e, 0x1e, 0x1e, 0x1e, 0x1e, 0x1e, 0x1e, 0x1e },
 { 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f }
};

// Настройка
void setup() {
// Создание символов из массива 
int charBitmapSize = (sizeof(charBitmap ) / sizeof (charBitmap[0]));  
// Последовательный порт
Serial.begin(9600);
// Аналоговый вход
pinMode(A0,INPUT);
// Запись созданных символов в дисплей
lcd.begin(20,4);

   for ( int i = 0; i < charBitmapSize; i++ )
   {
      lcd.createChar ( i, (uint8_t *)charBitmap[i] );
   }

// Настройка подсветки
lcd.setBacklightPin(BACKLIGHT,POSITIVE);
lcd.setBacklight(HIGH);
// Подготовка надписей
lcd.clear();
lcd.setCursor(0,1);
lcd.print("0   1   2   3   4  5");

}

// Основной код
void loop() {
  // Переменные
  char A0str[20]="" ;
  char str4[20]="" ;
  int A0val;
  float barl;
  // Считывание значения напряжения
  A0val=analogRead(A0);
  // Расчет напряжения и вывод на первую строку
  lcd.home();
  lcd.setCursor(0,0);
  sprintf(A0str, "U = %4d mV", int(A0val*4.883));
  lcd.print(A0str);
  // И в последовательный порт
  Serial.println(A0str);
  // Формирования полоски.
  lcd.setCursor(0,2);
  // Количество закрашенных мест
  barl = 20.0/1024.0*float(A0val);
  // Вывод
  for ( int i = 0; i < int(barl); i++ )
  {
    lcd.print(char(4));
  }
  // Определение и вывод
  // конца полоски
  int tmp = int(A0val*4.883) % 250;
  int partchar = tmp / 50;
  if (partchar > 0) {
  lcd.print(char(partchar));
  } else {
    lcd.print(" ");
  }
  // И добивание пробелами хвоста
  for ( int j = barl+2; j < 21; j++ )
  {
    lcd.print(" ");
  }
  // Задержка переде следующим замером
  delay(100);
}

