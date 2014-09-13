#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

#define CSNPIN 10
#define CEPIN 9

int senddata;

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
  radio.openWritingPipe(pipes[0]);
  radio.openReadingPipe(1,pipes[1]);
  radio.startListening();
  radio.printDetails();
  senddata = 0;
  Serial.println("This is SENDER");
}

void loop() {
  senddata++;
  bool ok;
  radio.stopListening();
  ok = radio.write(&senddata, sizeof(int)); 
  if (ok)
  {
    Serial.print("Sended packet #");
    Serial.println(senddata);
  } else {
    Serial.print("ERROR: not sended #");
    Serial.println(senddata);
  }
  radio.startListening();
  delay(1000);
}

// Для вывода информации
int serial_putc( char c, FILE * )
{
  Serial.write( c );
  return c;
}
