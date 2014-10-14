#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define OLED_RESET 4
Adafruit_SSD1306 display(OLED_RESET);

void setup() {
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.clearDisplay();
  int h;
  int w;
  h = display.height();
  w = display.width();
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(0,0);
  display.print("H: ");
  display.println(h);
  display.print("W: ");
  display.println(w);
  display.print("Hello!");
  display.display();
}

void loop() {
  // put your main code here, to run repeatedly:

}
