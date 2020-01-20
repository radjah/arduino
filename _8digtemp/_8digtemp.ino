#include <CyberLib.h>
#include <LedControl.h>
#include <DallasTemperature.h>
#include <OneWire.h>


LedControl mydisplay = LedControl(A0, A2, A1, 1);

// Температура
OneWire dswire(3);
DallasTemperature sensors(&dswire);
DeviceAddress dsaddr;

void setup() {
  //Serial.begin(9600,SERIAL_8N2);
  mydisplay.shutdown(0, false);  // turns on display
  mydisplay.setIntensity(0, 5); // brightest

  sensors.getAddress(dsaddr, 0);
  //printAddress(dsaddr);
  //Serial.println();
  sensors.setResolution(dsaddr, 12);
}

void loop() {
  delay_ms(250);
  unsigned int tempc;
  sensors.requestTemperatures();
  tempc = int(sensors.getTempC(dsaddr)*100);
  /*
  Serial.println(tempc);
  Serial.println((tempc % 1000000 - tempc % 10000) / 10000);
  Serial.println((tempc % 10000 - tempc % 100) / 100);
  Serial.println(tempc % 100);
  */
  print2dig (((tempc % 1000000 - tempc % 10000) / 10000), 5, 4, false);
  print2dig ((tempc % 10000 - tempc % 100) / 100, 3, 2, true);
  print2dig (tempc % 100, 1, 0, false);
}

void print2dig (int dig, int pl1, int pl2, bool dot) {
  mydisplay.setDigit(0, pl2, dig % 10, dot);
  mydisplay.setDigit(0, pl1, (dig % 100 - dig % 10) / 10, false);
}

// Вывод адреса в Serial
/*
void printAddress(DeviceAddress deviceAddress)
{
  for (uint8_t i = 0; i < 8; i++)
  {
    if (deviceAddress[i] < 16) Serial.print("0");
    Serial.print(deviceAddress[i], HEX);
  }
}
*/
