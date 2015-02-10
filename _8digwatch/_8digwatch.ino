#include <CyberLib.h>
#include <Wire.h>
#include <LedControl.h>
#include <RTClib.h>

#define TIMESPAN 3
//#define SETTIME

RTC_DS1307 rtc;
LedControl mydisplay = LedControl(A0, A2, A1, 1);

void setup() {
  Wire.begin();
  // Часы
  rtc.begin();
#ifdef SETTIME
  rtc.adjust(DateTime(__DATE__, __TIME__) - TimeSpan(TIMESPAN * 3600));
#endif
  D13_In;
  D13_Low;
  rtc.writeSqwPinMode(SquareWave1HZ);
  mydisplay.shutdown(0, false);  // turns on display
  mydisplay.setIntensity(0, 5); // 15 = brightest
}

void loop() {
  DateTime MSK = rtc.now() + TIMESPAN * 3600;
  print2dig (MSK.hour(), 5, 4, true);
  print2dig (MSK.minute(), 3, 2, true);
  print2dig (MSK.second(), 1, 0, false);
  delay_ms(100);

}

void print2dig (int dig, int pl1, int pl2, bool dot) {
  mydisplay.setDigit(0, pl2, dig % 10, dot);
  mydisplay.setDigit(0, pl1, (dig % 100 - dig % 10) / 10, false);
}
