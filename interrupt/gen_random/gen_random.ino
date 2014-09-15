#include <CyberLib.h>

void setup() {
  // D13 на ввод. Внешнее управление диодом.
  D13_In;
  D13_Low;
  // D2 на вывод. Управление оптопарой.
  D2_Out;
}

void loop() {
  // Случайные импульсы.
  D2_High;
  delay_us(100);
  D2_Low;
  delay_us(100);
}

