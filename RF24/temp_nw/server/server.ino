// Сервер Mega

#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

#define CSNPIN 53
#define CEPIN 9

struct sendtemp {
  float outtemp;
  float intemp;
};

sendtemp st;

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
  radio.openWritingPipe(pipes[0]);
  radio.openReadingPipe(1, pipes[1]);
  radio.startListening();
  Serial.println("This is SERVER");
  st.outtemp = 0.0;
  st.intemp = 0.0;
}

void loop() {
  st.outtemp++;
  st.intemp++;
  bool sendok;
  radio.stopListening();
  sendok = radio.write(&st, sizeof(sendtemp));
  if (sendok)
  {
    Serial.println("Sended!");
  } else {
    Serial.println("ERROR: not sended!");
  }
  radio.startListening();
  delay(1000);
}
