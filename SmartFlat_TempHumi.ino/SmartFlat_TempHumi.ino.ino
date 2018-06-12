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
#include <DHT.h>



#define DHTPIN 2     // what digital pin the DHT22 is conected to
#define DHTTYPE DHT22   // there are multiple kinds of DHT sensors

DHT dht(DHTPIN, DHTTYPE);

/* ---------- DO NOT EDIT ANYTHING ABOVE THIS LINE ---------- */

//Only edit the settings in this section

/* WIFI Settings */
// Name of wifi network
const char* ssid = "Tonic";

// Password to wifi network
const char* password = "fetthemligt";

/* Web Updater Settings */
// Host Name of Device
const char* host = "TempHumi1";

// Path to access firmware update page (Not Neccessary to change)
const char* update_path = "/firmware";

// Username to access the web update page
const char* update_username = "admin";

// Password to access the web update page
const char* update_password = "Admin";

/* MQTT Settings */
// Topic which listens for commands
char* outTopic = "SmartFlat/TempHumi1";
char* outTopic2 = "SmartFlat/TempHumi2";
char* outTopic3 = "SmartFlat/TempHumi3";

//MQTT Server IP Address
const char* server = "192.168.0.107";

//Unique device ID
const char* mqttDeviceID = "HampDaChamp-SmartFlat_TempHumi1";



/* ---------- DO NOT EDIT ANYTHING BELOW THIS LINE ---------- */

ESP8266WebServer httpServer(80);
ESP8266HTTPUpdateServer httpUpdater;

WiFiClient net;
MQTTClient client;

unsigned long lastMillis = 0;

int potPin = A0;


void connect() 
{
  while (WiFi.status() != WL_CONNECTED) 
  {
    delay(1000);
  }

  while (!client.connect(mqttDeviceID)) 
  {
    delay(1000);
  }
}
void setup() {
  Serial.begin(115200);
  Serial.setTimeout(2000);

  pinMode(potPin,INPUT);

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


int timeSinceLastRead = 0;

void loop() {
  
  client.loop();
  delay(10);

  if(!client.connected()) {
    connect();
  }

  httpServer.handleClient();


  // Report every 2 seconds.
  if(timeSinceLastRead > 5000) {
    // Reading temperature or humidity takes about 250 milliseconds!
    // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
    float h = dht.readHumidity();
    // Read temperature as Celsius (the default)
    float t = dht.readTemperature();


    // Check if any reads failed and exit early (to try again).
    if (isnan(h) || isnan(t)) {
      Serial.println("Failed to read from DHT sensor!");
      timeSinceLastRead = 0;
      return;
    }

    // Compute heat index in Celsius (isFahreheit = false)
    float hic = dht.computeHeatIndex(t, h, false);

    int val;                //Create an integer variable
    double tempU;            //Variable to hold a temperature value
    val=analogRead(0);      //Read the analog port 0 and store the value in val
    tempU=Thermister(val);   //Runs the fancy math on the raw analog value
  
    Serial.print("Humidity: ");
    Serial.print(h);
    Serial.print(" %\t");
    Serial.print("Temperature: ");
    Serial.print(t);
    Serial.print(" *C ");
    Serial.print("Outside Temperature: ");
    Serial.print(tempU);
    Serial.println(" *C ");

  String temp = (String)t;
  String humi = (String)h;
  String tempU2 = (String)tempU;
  client.publish(outTopic, temp);
  delay(100);
  client.publish(outTopic2, humi);
  delay(50);
  client.publish(outTopic3, tempU2);
  delay(50);


  timeSinceLastRead = 0;

  }
  
  delay(100);
  timeSinceLastRead += 100;  

}

double Thermister(int RawADC) {  //Function to perform the fancy math of the Steinhart-Hart equation
 double Temp;
 Temp = log(((10240000/RawADC) - 10000));
 Temp = 1 / (0.001129148 + (0.000234125 + (0.0000000876741 * Temp * Temp ))* Temp );
 Temp = Temp - 273.15;              // Convert Kelvin to Celsius
 //Temp = (Temp * 9.0)/ 5.0 + 32.0; // Celsius to Fahrenheit - comment out this line if you need Celsius
 return Temp;
}

void messageReceived(String &topic, String &payload) {

}

/*
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
  
}
*/
