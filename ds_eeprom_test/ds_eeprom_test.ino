#include <EEPROM2.h>

#include <OneWire.h>
#include <DallasTemperature.h>

OneWire oneWire(2);
DallasTemperature sensors(&oneWire);
DeviceAddress DS;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Serial.print("Locating devices...");
  sensors.begin();
  Serial.print("Found ");
  Serial.print(sensors.getDeviceCount(), DEC);
  Serial.println(" devices.");
  if (sensors.getDeviceCount() > 0) {
    EEPROM_write(0, sensors.getDeviceCount());
    sensors.getAddress(DS, 0);
    for (int i = 1; i < 9; i++) {
      EEPROM_write(i, DS[i - 1]);
      if (DS[i - 1] < 10) Serial.print("0");
      Serial.print(DS[i - 1], HEX);
    }
    Serial.println("");
    Serial.println("Done!");
  } else {
    byte c;
    for (int i = 1; i < 9; i++) {
      EEPROM_read(i, c);
      if (c < 10) Serial.print("0");
      Serial.print(c, HEX);
    }
  }

}

void loop() {
  // put your main code here, to run repeatedly:

}
