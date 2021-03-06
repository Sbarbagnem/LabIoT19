#include <math.h>

const int B = 4275;               // B value of the thermistor
const int R0 = 100000;            // R0 = 100k
const int pinTempSensor = A0;     // Grove - Temperature Sensor connect to A0

int BUTTON = 2; 
int val;
int d;

int triggerPort = 9;
int echoPort = 10;

#include <Wire.h>
#include "rgb_lcd.h"
rgb_lcd lcd;

#include "ThingSpeak.h"

#include <SPI.h>
#include <WiFi101.h>
#include "arduino_secrets.h"

IPAddress ip(149, 132, 182, 63);
IPAddress subnet(255, 255, 255, 0);
IPAddress dns(149, 132, 2, 3);
IPAddress gateway(149, 132, 182, 1);

WiFiClient client;
// WiFiServer server(80);

char ssid[] = SECRET_SSID;        // your network SSID (name)
char pass[] = SECRET_PASS;
int keyIndex = 0;                 // your network key Index number (needed only for WEP)
int status = WL_IDLE_STATUS;

// THINGSPEAK
unsigned long myChannelNumber = 755526;
const char * myWriteAPIKey = "DWNAXXAFDYP0D8XG";

void setup(){
  Serial.begin(9600);
  WiFi.begin(ssid,pass);
  lcd.begin(16, 2);
  pinMode(BUTTON, INPUT);
  ThingSpeak.begin(client);
  //impostazione dei pin
  pinMode(triggerPort, OUTPUT);
  pinMode(echoPort, INPUT);
}

void loop(){
  val = digitalRead(BUTTON); 
  delay(2000);
  WiFi.config(ip, dns, gateway, subnet);
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
  if(val == HIGH){
    int a = analogRead(pinTempSensor);
    float R = 1023.0/a-1.0;
    R = R0*R;
    float f = 1.0/(log(R/R0)/B+1/298.15)-273.15; // convert to temperature via datasheet

    Serial.print("temperature = ");
    Serial.println(f);
    delay(100);
    d = distanza();
    if(d > 40){
      Serial.print(">40");
    }else{
      Serial.print("<=40");
    }
    ThingSpeak.setField(1,f);
    ThingSpeak.setField(2,d);
    ThingSpeak.writeFields(myChannelNumber, myWriteAPIKey);
  
    Serial.print(" %\t ");
    Serial.print("Temperature: ");
    Serial.print(f);
  
    lcd.setCursor(0,0);
    lcd.print("Temp:");
    lcd.setCursor(7,0);
    lcd.setCursor(0,1);
    lcd.print(f);
 }
  //DELAY THINGSPEAK
  //delay(20000);
 delay(5000);
 lcd.clear();

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
}

long distanza() // programmazione sensore sfr-05
{
  //porta bassa l'uscita del trigger
  digitalWrite( triggerPort, LOW);
  delayMicroseconds(2);
  //invia un impulso di 10microsec su trigger
  digitalWrite(triggerPort, HIGH);
  delayMicroseconds(10);
  digitalWrite(triggerPort,LOW);
  // prende in ingresso il segnale nella porta echo e tramite un calcolo trova distanza (r)
  long duration = pulseIn( echoPort, HIGH); 
  long r = 0.034 * duration / 2;
  return r;   
}
