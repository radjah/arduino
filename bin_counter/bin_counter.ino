#include <LedControl.h>

// inputs: DIN pin, CLK pin, LOAD pin. number of chips
LedControl mydisplay = LedControl(A0, A2, A1, 1);
byte disp;

void setup() {
  mydisplay.shutdown(0, false);  // turns on display
  mydisplay.setIntensity(0, 5); // 15 = brightest
  disp = 0;
}

void loop() {
  displaybin(disp);
  disp++;
  delay(500);
}

void displaybin(byte dec) {
  for (int i = 0; i <= 7; i++) {
    byte chplace = dec >> i & B00000001;
    mydisplay.setDigit(0, i, int(chplace), false);
  }
}
