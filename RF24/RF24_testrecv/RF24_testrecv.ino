#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

#define CSNPIN 10
#define CEPIN 9

// Радио
RF24 radio(CEPIN, CSNPIN);

// Пайпы
const uint64_t pipes[2] = { 0xF0F0F0F0E1LL, 0xF0F0F0F0D2LL };

void setup() {
  // Дебаг
  Serial.begin(57600);
  fdevopen( &serial_putc, 0 );
  // Включаем радио
  radio.begin();
  // Настройка
  radio.setRetries(15, 15);
  radio.setAutoAck(1);
  // Пайпы на запись и чтение
  radio.openWritingPipe(pipes[1]);
  radio.openReadingPipe(1,pipes[0]);
  // Слушаем
  radio.startListening();
  radio.printDetails();
  Serial.println("This is RECIVER");
}

void loop() {
  if ( radio.available() )
  {
    bool done = false;
    int senddata;
    while (!done)
    {
      done = radio.read( &senddata, sizeof(int) );
      Serial.print("#");
      Serial.println(senddata);
      delay(20);
    };
  };
  delay(100);
}

// Для вывода информации
int serial_putc( char c, FILE * )
{
  Serial.write( c );

  return c;
}

