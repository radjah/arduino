// Сервер Mega

#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

#define CSNPIN 10
#define CEPIN 9

struct sendtemp {
  float outtemp;
  float intemp;
};

// Радио
RF24 radio(CEPIN, CSNPIN);

// Пайпы
const uint64_t pipes[2] = { 0xF0F0F0F0E1LL, 0xF0F0F0F0D2LL };

void setup() {
  Serial.begin(9600);
  // Включаем радио
  radio.begin();
  // Настройка
  radio.setRetries(15, 15);
  radio.setAutoAck(1);
  // Пайпы на запись и чтение
  radio.openWritingPipe(pipes[1]);
  radio.openReadingPipe(1, pipes[0]);
  radio.startListening();
  Serial.println("This is CLIENT");
}

void loop() {
  if (radio.testCarrier())
    Serial.println("Carrier OK!");
  else
    Serial.println("NO CARRIER!");
  if ( radio.available() )
  {
    bool done = false;
    sendtemp st;
    while (!done)
    {
      done = radio.read( &st, sizeof(sendtemp) );
      Serial.println("Recived!");
      Serial.print("Outtemp: ");
      Serial.println(st.outtemp);
      Serial.print("Intemp: ");
      Serial.println(st.intemp);
      delay(20);
    };
  };
  delay(100);
}
