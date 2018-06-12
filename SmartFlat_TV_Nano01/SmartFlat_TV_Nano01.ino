/*
   IRremote: IRsendDemo - demonstrates sending IR codes with IRsend
   An IR LED must be connected to Arduino PWM pin 3.
   Version 0.1 July, 2009
   Copyright 2009 Ken Shirriff
   http://arcfn.com
*/


#include <IRremote.h>

IRsend irsend;

void setup()
{
  Serial.begin(9600);
}

void loop() {

  if (Serial.available()) {

    //read serial as a character
    char ser = Serial.read();
    Serial.println(ser);
    //NOTE because the serial is read as "char" and not "int", the read value must be compared to character numbers
    //hence the quotes around the numbers in the case statement
    switch (ser) {
      case '0':  // the OFF command from the ESP and OpenHab
        irsend.sendRC6(0xC, 20);
        Serial.println("off");
        delay(10);
        break;
      case '1':  // the ON command from the ESP and OpenHab
        irsend.sendRC6(0x1000C, 20);
        Serial.println("on");
        delay(10);
      case '2': // the volume UP command from the ESP and OpenHab
        for (int i = 0; i < 5; i++) {
          irsend.sendRC6(0x10, 20);
          Serial.println("up: ");
          Serial.println(i);
          delay(700);
        }
        break;
      case '3': // the volume DOWN command from the ESP and OpenHab /
        for (int i = 0; i < 3; i++) {
          irsend.sendRC6(0x10011, 20);
          Serial.println("down: ");
          Serial.println(i);
          delay(700);
        }
        break;
    }
  }
}
