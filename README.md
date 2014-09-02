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

## check_optron ##
Простенький скетч для проверки оптопары.

- Вход: D2 - GND через резистор 4,7 кОм
- Выход: 3.3V - D13 (LED)

Использовалась библиотека [CyberLib](http://cyber-place.ru/showthread.php?t=550).

## test_int ##

Попытка разобраться с прерываниями. Nano через оптопару генерирует прерывание на Mega.
