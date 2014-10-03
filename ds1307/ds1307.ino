#include <Wire.h>
#include "RTClib.h"

RTC_DS1307 rtc;

void setup () {
  Serial.begin(9600);
  Wire.begin();
  rtc.begin();
}

void loop () {
    DateTime now = rtc.now();
    DateTime MSK (now.unixtime() + 4 * 3600);
    char dt[18] = "";
    sprintf(dt, "%02d/%02d/%02d %02d:%02d:%02d", MSK.year(),
                                                 MSK.month(),
                                                 MSK.day(),
                                                 MSK.hour(),
                                                 MSK.minute(),
                                                 MSK.second());
    Serial.print("GMT+4: ");
    Serial.print(dt);
    Serial.println();
    Serial.println();
    delay(3000);
}
