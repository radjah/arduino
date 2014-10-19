#include <CyberLib.h>

#define LATCH  12  //pin 12 of BBFuino connect to RCK of 8x7segment module 
#define CLOCK  11  //pin 11 of BBFuino connect to SCK of 8x7segment module 
#define DATA   10  //pin 10 of BBFuino connect to DIO of 8x7segment module 
#define LEFT   0  // define the value for left justify
#define RIGHT  1  // define the value for right justify
#define  MultiplexDelay  0  //delay for multiplexing between digit on 8x7segment module

byte anode[8] = { 0b10000000,  //digit 1 from right
                  0b01000000,  //digit 2 from right
                  0b00100000,  //digit 3 from right
                  0b00010000,  //digit 4 from right
                  0b00001000,  //digit 5 from right
                  0b00000100,  //digit 6 from right
                  0b00000010,  //digit 7 from right
                  0b00000001   //digit 8 from right
                };

//array for decimal number, it is the cathode, please refer to the datasheet.
//therefore a logic low will activete the particular segment
//PGFEDCBA, segment on 7 segment, P is the dot
byte cathode[12] = {0b11000000,  // 0
                    0b11111001,  // 1
                    0b10100100,  // 2
                    0b10110000,  // 3
                    0b10011001,  // 4
                    0b10010010,  // 5
                    0b10000010,  // 6
                    0b11111000,  // 7
                    0b10000000,  // 8
                    0b10010000,  // 9
                    0b01111111,  //dot
                    0b11111111   //blank
                   };


void setup() {
  pinMode(LATCH, OUTPUT);
  pinMode(CLOCK, OUTPUT);
  pinMode(DATA, OUTPUT);
  digitalWrite(LATCH, HIGH);
//  delay_ms(1000);
}
//   1       0  01001001
//  6 2     0   12345678
//   7       0
//  5 3       0
//   4  8    0
void loop() {
  /*  display8x7segment(10, 11, 12, 0b10000000, 0b11111001);
    display8x7segment(10, 11, 12, 0b01000000, 0b10100100);
    delay(300);
  for (byte i = 0; i < 8; i++)
  {
    for (byte k = 0; k < 11; k++)
    {
      display8x7segment(DATA, CLOCK, LATCH, anode[i], cathode[k]);
      delay(300);
    }
  }*/
  displayNumber8x7segment(RIGHT, 100500);
//  delay(1000);  //delay 1 second
}

void display8x7segment(byte datapin, byte clockpin, byte latchpin, byte digit, byte number)
{
  digitalWrite(latchpin, LOW);
  shiftOut(datapin, clockpin, MSBFIRST, digit); // clears the right display
  shiftOut(datapin, clockpin, MSBFIRST, number); // clears the left display
  digitalWrite(latchpin, HIGH);
}


void displayNumber8x7segment(byte justify, unsigned long value)
{

   byte decimal[8] = {0};   
   value = value % 100000000;  //ensure the value is within 8 digits only
   decimal[7] = value / 10000000;  //extract digit 7 from value
   value = value % 10000000;       //extract the rest of 7 digit value
   decimal[6] = value / 1000000;
   value = value % 1000000;
   decimal[5] = value / 100000;
   value = value % 100000;
   decimal[4] = value / 10000;
   value = value % 10000;
   decimal[3] = value / 1000;
   value = value % 1000;
   decimal[2] = value / 100;
   value = value % 100;
   decimal[1] = value / 10;
   decimal[0] = value % 10;
   byte zero = 0;
   if (justify == RIGHT)
   {   
     for(byte e = 8; e > 0 ; e --)
     {
       if(zero == 0)
       {
         if(decimal[e-1] != 0)          
         {
           display8x7segment(DATA, CLOCK, LATCH, anode[e-1], cathode[decimal[e-1]]);
           zero = 1;     
         }
       }
       else display8x7segment(DATA, CLOCK, LATCH, anode[e-1], cathode[decimal[e-1]]);
     
     delay(MultiplexDelay); 
     }
   }
   else  //if justify == left
   { 
     byte d = 0;     
     for(byte e = 8; e > 0; e --)
     {       
       if(zero == 0)
       {
         if(decimal[e-1] != 0)         
         {
           display8x7segment(DATA, CLOCK, LATCH, anode[7], cathode[decimal[e-1]]);
           zero = 1;
           d ++;     
           delay(MultiplexDelay); 
         }
       }
       else 
       {
         display8x7segment(DATA, CLOCK, LATCH, anode[7-d], cathode[decimal[e-1]]);
         d ++;
         delay(MultiplexDelay); 
       }     
       
     }
     
   }
}
