#include <CyberLib.h>

void setup() {
  D13_In;
  D13_Low;
  D2_Out;
}

void loop() {
  D2_High;
  delay_ms(1000);
  D2_Low;
  delay_ms(1000);
}

