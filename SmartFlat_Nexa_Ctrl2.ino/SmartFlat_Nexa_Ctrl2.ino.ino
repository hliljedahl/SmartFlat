/*
   HomeEasyAdv Library transmit demo
   Simple example to demonstrate turning on and off devices
   The example turns on the specific channel for 3 seconds then turns it off
   By Andrew Lindsay
*/
#include <HomeEasyAdv.h>

// Set the TX and RX pins you are using
#define TXPIN 3
#define RXPIN 4

//This can be the same as an existing remote control, makes setting up the HE302S sockets easier
// use the receiver test program from the playground to determine yours.
#define REMOTE_ADDRESS 1010123

HomeEasyAdv homeeasy = HomeEasyAdv(RXPIN, TXPIN, REMOTE_ADDRESS );

#include <SoftwareSerial.h>
SoftwareSerial ESPserial(10, 11); // RX | TX

void setup() {
  Serial.begin(115200);
  ESPserial.begin(115200);
  /*

    Serial.println("On");
    digitalWrite( LEDPIN, HIGH );
    homeeasy.on( CHANNEL );
    Serial.println("On");
    homeeasy.on( CHANNEL );
    Serial.println("On");
    homeeasy.on( CHANNEL );
    Serial.println("On");
    homeeasy.on( CHANNEL );
    delay(3000);

    Serial.println("Off");
    digitalWrite( LEDPIN, LOW );
    homeeasy.off( CHANNEL );

  */
}

void loop() {

  if ( ESPserial.available() )   {
    char input = ESPserial.read();
    Serial.write(input);
    switch (input) {
      case '1':
        //do something when var equals 1
        Serial.println("ONE");
        homeeasy.on(1);
        homeeasy.on(1);
        break;
      case '2':
        //do something when var equals 2
        homeeasy.on(2);
        homeeasy.on(2);
        Serial.println("TWO");
        break;
      case '3':
        //do something when var equals 2
        homeeasy.on(3);
        homeeasy.on(3);
        Serial.println("TRHEE");
        break;
      case '4':
        //do something when var equals 1
        Serial.println("ONE_OFF");
        homeeasy.off(1);
        homeeasy.off(1);
        break;
      case '5':
        //do something when var equals 2
        homeeasy.off(2);
        homeeasy.off(2);
        Serial.println("TWO_OFF");
        break;
      case '6':
        //do something when var equals 2
        homeeasy.off(3);
        homeeasy.off(3);
        Serial.println("TRHEE_OFF");
        break;
      default:
        // if nothing else matches, do the default
        // default is optional
        break;
    }
  }
}
