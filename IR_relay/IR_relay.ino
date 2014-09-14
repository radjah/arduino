
#include <CyberLib.h>

/*
 * IRremote: IRrecvDemo - demonstrates receiving IR codes with IRrecv
 * An IR detector/demodulator must be connected to the input RECV_PIN.
 * Version 0.1 July, 2009
 * Copyright 2009 Ken Shirriff
 * http://arcfn.com
 */

#include <IRremote.h>
#include "carremote.h"

#define R1ON    B_1 // Кнопка 1
#define R1OFF   B_4 // Кнопка 2
#define R2ON    B_2 // Кнопка 4
#define R2OFF   B_5 // Кнопка 5
#define BOTHON  B_3 // Кнопка 3
#define BOTHOFF B_6 // Кнопка 6

int RECV_PIN = 11;

IRrecv irrecv(RECV_PIN);

decode_results results;

void setup()
{
  irrecv.enableIRIn(); // Start the receiver
  D2_Out;
  D3_Out;
  D2_High;
  D3_High;
}

void loop() {
  if (irrecv.decode(&results)) {
    switch (results.value) {
      case R1ON:
        D2_Low;
        break;
      case R1OFF:
        D2_High;
        break;
      case R2ON:
        D3_Low;
        break;
      case R2OFF:
        D3_High;
        break;
      case BOTHON:
        D2_Low;
        D3_Low;
        break;
      case BOTHOFF:
        D2_High;
        D3_High;
        break;
    }
    irrecv.resume(); // Receive the next value
  }
  delay(100);
}
