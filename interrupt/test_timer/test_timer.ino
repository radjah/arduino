#include <TimerOne.h>
#include <CyberLib.h>
#include <Wire.h>
#include <LCD.h>
#include <LiquidCrystal_I2C.h>


// Порты LCD
#define LCD_I2C_ADDR    0x27
#define BACKLIGHT     3
#define LCD_EN  2
#define LCD_RW  1
#define LCD_RS  0
#define LCD_D4  4
#define LCD_D5  5
#define LCD_D6  6
#define LCD_D7  7

LiquidCrystal_I2C       lcd(LCD_I2C_ADDR, LCD_EN, LCD_RW, LCD_RS, LCD_D4, LCD_D5, LCD_D6, LCD_D7);

volatile long int count = 0;     // Счетчик
volatile long int oldcount = 0;  // Для вычисления скорости
volatile float speed = 0;   // Значение скорости

void setup() {
  // D3 на ввод
  D3_In;
  D3_High;
  // Прерывание по нарастанию
  attachInterrupt(1, D2Int, RISING);
  // Таймер на 4 секунды
  Timer1.initialize(4000000);
  Timer1.attachInterrupt(Calcspeed);
  // Запуск дисплея
  lcd.begin(20, 4);
  lcd.setBacklightPin(BACKLIGHT, POSITIVE);
  lcd.setBacklight(HIGH);
  // Шаблон
  lcd.clear();
  lcd.setCursor(0,1);
  lcd.print("Speed: ");
  lcd.home();
  Serial.begin(9600);
}

void loop() {
  lcd.home();
  // Буферы для вывода на дисплей
  char cnt[20] = "";  // Счетчик
  char spd[13] = "";  // Скорость
  // Счетчик в строку
  sprintf(cnt, "Count: %13lu", count);
  lcd.print(cnt);
  // Скорость в строку
  dtostrf(speed, 13, 2, spd);
  lcd.setCursor(7,1);
  lcd.print(spd);
  Serial.print(count);
  Serial.print("   ");
  Serial.println(speed);
  // Задержка
  delay_ms(1000);
}
void D2Int() {
  // Приращение счетчика по прерыванию
  count++;
}

void Calcspeed(){
  // Вычисление скорости
  speed = float(count - oldcount)/4.0;
  oldcount = count;
}
