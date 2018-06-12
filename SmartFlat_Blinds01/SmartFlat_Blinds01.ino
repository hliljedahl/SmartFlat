/*
   Device Title: MK-SonoffPowerStrip
   Device Description: MQTT Sonoff Power Strip Control
   Device Explanation: The MQTT server sends a message to the device and
                       based on the message the device turns on a relay, which
                       is attached to an outlet
   Device information: https://www.MK-SmartHouse.com/sonoff-power-strip

   Author: Matt Kaczynski
   Website: http://www.MK-SmartHouse.com

   Code may only be distrbuted through http://www.MK-SmartHouse.com any other methods
   of obtaining or distributing are prohibited
   Copyright (c) 2016-2017

   Note: After flashing the code once you can remotely access your device by going to http://HOSTNAMEOFDEVICE.local/firmware
   obviously replace HOSTNAMEOFDEVICE with whatever you defined below. The user name and password are also defined below.
*/
#include <ESP8266WiFi.h>
#include <MQTTClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <ESP8266HTTPUpdateServer.h>

/* ---------- DO NOT EDIT ANYTHING ABOVE THIS LINE ---------- */

//Only edit the settings in this section

/* WIFI Settings */
// Name of wifi network
const char* ssid = "Tonic";

// Password to wifi network
const char* password = "fetthemligt";

/* Web Updater Settings */
// Host Name of Device
const char* host = "SmartBlinds01";

// Path to access firmware update page (Not Neccessary to change)
const char* update_path = "/firmware";

// Username to access the web update page
const char* update_username = "admin";

// Password to access the web update page
const char* update_password = "Admin";

/* MQTT Settings */
// Topic which listens for commands
char* subscribeTopic = "SmartFlat/Window/SmartBlinds01";

//MQTT Server IP Address
const char* server = "192.168.0.107";

//Unique device ID
const char* mqttDeviceID = "HampDaChamp-SmartFlat-Blinds01";

const int stepPin = 4; 
const int dirPin = 2; 
const int butPin = 5;
const int halPin = 14;
      int stats = 0;
      int maxLength = 800;
      int currentPos = 0;
      int travel = 0;
      int dirr = 0;

/* ---------- DO NOT EDIT ANYTHING BELOW THIS LINE ---------- */

ESP8266WebServer httpServer(80);
ESP8266HTTPUpdateServer httpUpdater;

WiFiClient net;
MQTTClient client;

unsigned long lastMillis = 0;

bool channel1 = false;
bool channel2 = false;

void connect();

void setup() {
  Serial.begin(250000);

  WiFi.mode(WIFI_STA);

  WiFi.begin(ssid, password);
  client.begin(server, net);
  client.onMessage(messageReceived);

  connect();

  MDNS.begin(host);

  httpUpdater.setup(&httpServer, update_path, update_username, update_password);
  httpServer.begin();

  MDNS.addService("http", "tcp", 80);

  pinMode(stepPin,OUTPUT); 
  pinMode(dirPin,OUTPUT);
  pinMode(butPin,INPUT);
  pinMode(halPin,INPUT);

}

void connect() {
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
  }

  while (!client.connect(mqttDeviceID)) {
    delay(1000);
  }

  client.subscribe(subscribeTopic);
}

void loop() {
  client.loop();
  delay(10);

  if (!client.connected()) {
    connect();
  }
  //Serial.println("Update");
  httpServer.handleClient();

  if(digitalRead(butPin)){
    while(digitalRead(butPin)){
      delay(2);
    }
    Serial.println("Homing");
    //Home without hall effect.
    digitalWrite(dirPin,LOW);
    while (digitalRead(halPin)){
      digitalWrite(stepPin,HIGH); 
      delayMicroseconds(1500); 
      digitalWrite(stepPin,LOW); 
      delayMicroseconds(1500); 
    }
    Serial.println("Homed");
    currentPos = 0;
  }  
  if (travel){
    //for (int x = 0; x <= travel; x++) {
    digitalWrite(stepPin,HIGH); 
    delayMicroseconds(1500); 
    digitalWrite(stepPin,LOW); 
    delayMicroseconds(1500); 
    //Serial.println(x);
    //}
    travel--;
    if(dirr){
      currentPos--;
    }
    else{
      currentPos++;
    }
    //Serial.println(currentPos);
  }    
    /* //Home without hall effect.
    digitalWrite(dirPin,LOW);
    travel = currentPos;
    
    Serial.print("Travel: ");
    Serial.println(travel);
    if (travel){
      for (int x = 0; x <= travel; x++) {
      digitalWrite(stepPin,HIGH); 
      delayMicroseconds(1500); 
      digitalWrite(stepPin,LOW); 
      delayMicroseconds(1500); 
      //Serial.println(x);
      }
    }
    currentPos = 0;
    Serial.print("CurrentPos: ");
    Serial.println(currentPos);
  */
  
  
  
}

void messageReceived(String &topic, String &payload) {
  String msgString = payload;
  //Serial.println(msgString);
  int msgInt = msgString.toInt();

  int dest = map(msgInt, 0, 100, 0, maxLength);
 
  Serial.print("CurrentPos: ");
  Serial.println(currentPos);
  Serial.print("Input: ");
  Serial.println(msgInt);
  Serial.print("Destination: ");
  Serial.println(dest);
  
  if (currentPos < dest) {
    digitalWrite(dirPin,HIGH);
    travel = dest - currentPos;
    dirr = 0;
  }
  else if (currentPos > dest) {
    digitalWrite(dirPin,LOW);
    travel = currentPos - dest;
    dirr = 1;
  }
  else{
    travel = 0;
  }
  //currentPos = dest;
  Serial.print("Travel: ");
  Serial.println(travel);

}
