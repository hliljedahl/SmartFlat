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

const int dc1 = 2; 
const int dc2 = 0; 
const int butPin = 5;
const int halPin = 14;
      int stats = 0;
      int maxLength = 800;
      int currentPos = 0;
      int travel = 0;
      int dirr = 0;
      int up = 1;
      int reset = 1; 

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

  pinMode(dc1,OUTPUT); 
  pinMode(dc2,OUTPUT);
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

  /********************************************************************************************
   * ******************************************************************************************
  
   
  */  
}

void messageReceived(String &topic, String &payload) {
  String msgString = payload;
  //Serial.println(msgString);
  int msgInt = msgString.toInt();

  int dest = map(msgInt, 0, 100, 0, maxLength);
  if(reset == 1){
    
  }
  if(msgInt == 0){
    if(up == 1){
      digitalWrite(dc1, HIGH);
      digitalWrite(dc2, LOW);
      delay(2000);
      up = 0;
      digitalWrite(dc1, LOW);
    } 
  }
  if(msgInt == 1){
    if(up == 0){
      digitalWrite(dc1, LOW);
      digitalWrite(dc2, HIGH);
      delay(2000);
      up = 1;
      digitalWrite(dc2, LOW);
    }
  }
    
  Serial.print("CurrentPos: ");  
}
