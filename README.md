# Arduino #
---
Мои скетчи для Arduino Mega/Uno/Nano.

Во всех проектах, связанных с использованием дисплея, использует библиотека [LiquidCrystal](https://bitbucket.org/fmalpartida/new-liquidcrystal/downloads). 
## voltmeter ##
Вольтметр на 5 вольт.

- "+" - A0
- "-" - GND

## lcd_temp ##
Скелет для считывания показаний пирометра и вывода значений на ЖК-дисплей.

## bar_lcd ##
Скетч для вывода показаний датчиков BMP085/BMP180, DHT11 на ЖК-дисплей.

Для работы с датчиками используется следующие библиотеки:

- [Adafruit-BMP085-Library](https://github.com/adafruit/Adafruit-BMP085-Library) для BMP085/BMP180
- [DHT-sensor-library](https://github.com/adafruit/DHT-sensor-library) для DHT11


## kelvin_emul ##
Эмулятор вывода пирометра Кельвин Д.

## interrupt ##

Примеры для обработки прерываний.

## RF24 ##

Мои попытки совладать с модулем nRF24L01+

Использована библиотека [RF24](https://github.com/maniacbug/RF24).

## IR_relay ##

Управление двумя реле на цифровых выводах 2 и 3.
Использована библиотека [Arduino-IRremote](https://github.com/shirriff/Arduino-IRremote).

## Dallas_temp ##

Чтение температуры с датчиков DS18B20 и BMP180. Для доступа к датчику DS18B20 использована библиотека [Arduino-Temperature-Control-Library](https://github.com/milesburton/Arduino-Temperature-Control-Library).

Для работы с шиной 1wire использована библиотека [OneWire](http://www.pjrc.com/teensy/td_libs_OneWire.html).

## float_bt ##

Пересылка показаний датчика BMP180 с Nano на Uno. Канал организован через два адаптера HC-05.

## oled ##

Проба дисплея 128x64 точек на контроллере SSD1306. Использовались библиотеки [Adafruit-GFX-Library](https://github.com/adafruit/Adafruit-GFX-Library) и [Adafruit_SSD1306](https://github.com/adafruit/Adafruit_SSD1306).

## due ##

Попытки перенести старые скетчи на Due.