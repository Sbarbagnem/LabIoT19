#include <PubSubClient.h>
#include <math.h>
#include <SPI.h>
#include <WiFi101.h>
#include <Wire.h>
#include "rgb_lcd.h"
#include "arduino_secrets.h"

// pin setting
int BUTTON = 2;
const int pinTempSensor = A0;     // Grove - Temperature Sensor connect to A0
int triggerPort = 9;
int echoPort = 10;

// device variable
rgb_lcd lcd;

// another variable used in script
const int B = 4275;               // B value of the thermistor
const int R0 = 100000;            // R0 = 100k
const int pinTempSensor = A0;     // Grove - Temperature Sensor connect to A0
int val;
int d;
int id = 1;

// setting varibale of wifi connection
IPAddress ip(149, 132, 182, 63);
IPAddress subnet(255, 255, 255, 0);
IPAddress dns(149, 132, 2, 3);
IPAddress gateway(149, 132, 182, 1);
char ssid[] = SECRET_SSID;        // your network SSID (name)
char pass[] = SECRET_PASS;
int keyIndex = 0;                 // your network key Index number (needed only for WEP)
int status = WL_IDLE_STATUS;
WiFiClient qclient;

// setting MQTT variable
PubSubClient client(qclient);
const char* mqttServer = "macman"; // 127.0.0.1 macman è il nostro localhost
const int mqttPort = 1883;

void setup() {
  Serial.begin(9600);
  WiFi.begin(ssid, pass);
  lcd.begin(16, 2);
  pinMode(BUTTON, INPUT);
  pinMode(triggerPort, OUTPUT);
  pinMode(echoPort, INPUT);
  WiFi.config(ip, dns, gateway, subnet);
}

// function in loop to connect in lose network
void reconnected() {

  if (WiFi.status() != WL_CONNECTED) {
    Serial.print("Attempting to connect to SSID: ");
    Serial.println(SECRET_SSID);

    while (WiFi.status() != WL_CONNECTED) {
      WiFi.begin(ssid, pass); // Connect to WPA/WPA2 network. Change this line if using open or WEP network
      Serial.println(".");
      delay(10000);
    }

    Serial.println("\nConnected");
    // you're connected now, so print out the status:
    printWifiStatus();
  }

  // connection to broker MQTT
  client.setServer(mqttServer, mqttPort);
}

void loop() {

  // function for MQTT (?)
  client.loop();

  if (!client.connected()) {
    reconnected();
  }

  // function to set callback on topic subsribed
  //client.setCallback(callback);

  // publish a message roughly every second.
  if (millis() - lastMillis > 1000) {
    lastMillis = millis();
    client.publish("outTopic", "Hello world");
  }
}


String getWiFiStatus() {
  IPAddress ip = WiFi.localIP();
  String ipaddress = String(ip[0]) + String(".") + \
                     String(ip[1]) + String(".") + \
                     String(ip[2]) + String(".") + \
                     String(ip[3]) ;

  //String netInfo = WiFi.SSID();
  //netInfo = netInfo + " IP " + ipaddress;
  String netInfo = " IP " + ipaddress;
  return netInfo;


}

void printWifiStatus() {

  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);

  // print the received signal strength:
  long rssi = WiFi.RSSI();
  Serial.print("signal strength (RSSI):");
  Serial.print(rssi);
  Serial.println(" dBm");
  Serial.println("");
}

long distanza() // programmazione sensore sfr-05
{
  //porta bassa l'uscita del trigger
  digitalWrite( triggerPort, LOW);
  delayMicroseconds(2);
  //invia un impulso di 10microsec su trigger
  digitalWrite(triggerPort, HIGH);
  delayMicroseconds(10);
  digitalWrite(triggerPort, LOW);
  // prende in ingresso il segnale nella porta echo e tramite un calcolo trova distanza (r)
  long duration = pulseIn( echoPort, HIGH);
  long r = 0.034 * duration / 2;
  return r;
}
