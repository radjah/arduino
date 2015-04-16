#include <LedControl.h>

// inputs: DIN pin, CLK pin, LOAD pin. number of chips
LedControl lc = LedControl(7, 5, 6, 1);

void setup()
{
  // the zero refers to the MAX7219 number, it is zero for 1 chip
  lc.shutdown(0,false);// turn off power saving, enables display
  lc.setIntensity(0,8);// sets brightness (0~15 possible values)
  lc.clearDisplay(0);// clear screen
}
void loop()
{
  for (int a=8; a>=0; a--)
  {
    lc.setDigit(0,a,7-a,false);
    delay(100);
  }
  for (int a=8; a>=0; a--)
  {
    lc.setDigit(0,a,8,false);
    delay(100);
  }
  for (int a=8; a>=0; a--)
  {
    lc.setDigit(0,a,0,false);
    delay(100);
  }
  for (int a=8; a>=0; a--)
  {
    lc.setChar(0,a,' ',false);
    delay(100);
  }
  for (int a=8; a>=0; a--)
  {
    lc.setChar(0,a,'-',false);
    delay(100);
  }
  for (int a=8; a>=0; a--)
  {
    lc.setChar(0,a,' ',false);
    delay(100);
  }
}
