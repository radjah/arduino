#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <DallasTemperature.h>
#include <OneWire.h>
#include <Wire.h>
#include <Adafruit_BMP085.h>
#include "printf.h"
/*
#include <LCD.h>
#include <LiquidCrystal_I2C.h>

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

LiquidCrystal_I2C       lcd(LCD_I2C_ADDR, LCD_EN, LCD_RW, LCD_RS, LCD_D4, LCD_D5, LCD_D6, LCD_D7);

*/

#define CSNPIN 10
#define CEPIN 9

struct sendtemp {
  float outtemp;
  float intemp;
};

// Радио
RF24 radio(CEPIN, CSNPIN);

// Пайпы
const uint64_t pipes[2] = { 0xF0F0F0F0E1LL, 0xF0F0F0F0D2LL };


OneWire dswire(2);
DallasTemperature sensors(&dswire);
DeviceAddress dsaddr;
Adafruit_BMP085         bmp;

void setup() {
  Serial.begin(9600);
  printf_begin();
  Serial.println("Begin setup");
  bmp.begin();
  /*
  lcd.begin(20, 4);
  lcd.setBacklightPin(BACKLIGHT, POSITIVE);
  lcd.setBacklight(HIGH);
  lcd.clear();
  lcd.home();
  */
  sensors.getAddress(dsaddr, 0);
  /*
  lcd.print("Dev  :");
  printAddress(dsaddr);
  lcd.setCursor(0, 1);
  lcd.print("T('C):");
  */
  sensors.setResolution(dsaddr, 12);
  /*
  lcd.setCursor(0, 2);
  lcd.print("T('C):");
  */
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
  radio.printDetails();
  Serial.println("Setup finished!");
}

void loop() {
  sendtemp st;
  /*
  char tempcstr[15] = "";
  */
  float tempc;
  sensors.requestTemperatures();
  tempc = sensors.getTempC(dsaddr);
  st.outtemp = tempc;
  Serial.print("Outtemp: ");
  Serial.print(tempc);
  /*
  lcd.setCursor(6, 1);
  dtostrf(tempC, 14, 2, tempcstr);
  lcd.print(tempcstr);
  */
  tempc = bmp.readTemperature();
  st.intemp = tempc;
  Serial.print(" Intemp: ");
  Serial.println(tempc);
  /*
  lcd.setCursor(6, 2);
  dtostrf(tempC, 14, 2, tempcstr);
  lcd.print(tempcstr);
  */
  radio.stopListening();
  if (radio.testCarrier())
    Serial.println("Carrier OK!");
  else
    Serial.println("NO CARRIER!");
  bool ok;
  ok = radio.write(&st, sizeof(sendtemp));
  if (ok)
    Serial.println("Sended!");
  else
    Serial.println("ERROR: not sended!");
  radio.startListening();
  delay(100);
}

/*
void printAddress(DeviceAddress deviceAddress)
{
  for (uint8_t i = 0; i < 8; i++)
  {
    if (deviceAddress[i] < 16) Serial.print("0");
    lcd.print(deviceAddress[i], HEX);
  }
}
*/
