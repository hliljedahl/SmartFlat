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
const char* host = "NexaController1";

// Path to access firmware update page (Not Neccessary to change)
const char* update_path = "/firmware";

// Username to access the web update page
const char* update_username = "admin";

// Password to access the web update page
const char* update_password = "Admin";

/* MQTT Settings */
// Topic which listens for commands
char* subscribeTopic = "SmartFlat/Nexa";

//MQTT Server IP Address
const char* server = "192.168.0.107";

//Unique device ID
const char* mqttDeviceID = "HampDaChamp-SmartFlat_Nexa2";



/* ---------- DO NOT EDIT ANYTHING BELOW THIS LINE ---------- */

ESP8266WebServer httpServer(80);
ESP8266HTTPUpdateServer httpUpdater;

WiFiClient net;
MQTTClient client;

unsigned long lastMillis = 0;

void connect();

void setup() {
  Serial.begin(115200);

  WiFi.mode(WIFI_STA);

  WiFi.begin(ssid, password);
  client.begin(server, net);
  client.onMessage(messageReceived);

  connect();

  MDNS.begin(host);

  httpUpdater.setup(&httpServer, update_path, update_username, update_password);
  httpServer.begin();

  MDNS.addService("http", "tcp", 80);

//  pinMode(ledPin, OUTPUT);
//  digitalWrite(ledPin, LOW);
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

  httpServer.handleClient();

}

void messageReceived(String &topic, String &payload) {
  String msgString = payload;
  Serial.println(msgString);
 /* 
  if (msgString == "C1ON") {
  //  digitalWrite(ledPin, HIGH);
    Serial.println("on");
  }
  else if (msgString == "C1ON") {
    digitalWrite(ledPin, LOW);
    Serial.println("off");
  }
  else {
    int msgInt = msgString.toInt();
    //Serial.println(msgString);
    //Serial.println(msgInt);
    int ledOut;
    ledOut = map(msgInt, 0, 100, 0, 255);
    analogWrite(ledPin, ledOut);
    Serial.println(msgInt);
  }
  */
}
