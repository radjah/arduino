#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <DallasTemperature.h>
#include <OneWire.h>
#include <Wire.h>
#include <Adafruit_BMP085.h>

#define CSNPIN 10
#define CEPIN 9

struct sendtemp {
  float outtemp;
  float intemp;
  float pres;
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
  Serial.println("Begin setup");
//  bmp.begin();
  sensors.getAddress(dsaddr, 0);
  sensors.setResolution(dsaddr, 12);
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
  Serial.println("Setup finished!");
}

void loop() {
  sendtemp st;
  float tempc;
  sensors.requestTemperatures();
  tempc = sensors.getTempC(dsaddr);
  st.outtemp = tempc;
  Serial.print("Outtemp: ");
  Serial.print(tempc);
//  tempc = bmp.readTemperature();
//  st.intemp = tempc;
  st.intemp = 1.0;
  Serial.print(" Intemp: ");
  Serial.println(tempc);
//  tempc = bmp.readPressure() / 133.32;
//  st.pres = tempc;
  st.pres = 2.0;
  Serial.print(" Inpres: ");
  Serial.println(tempc);
  radio.stopListening();
  bool ok = radio.write(&st, sizeof(sendtemp));
  if (ok)
    Serial.println("Sended!");
  else
    Serial.println("ERROR: not sended!");
  radio.startListening();
  delay(100);
}

