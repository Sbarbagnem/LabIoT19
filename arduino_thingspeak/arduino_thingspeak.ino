#include <dht11.h>
dht11 DHT;
#define DHT11_PIN 4
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
unsigned long myChannelNumber = 752647;
const char * myWriteAPIKey = "WXQO8FW29QO5RRD9";

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
    int chk;
    chk = DHT.read(DHT11_PIN);  
    delay(500);// READ DATA
    // set the cursor to column 0, line 1
    // (note: line 1 is the second row, since counting begins with 0):
    // read humidity
    float h = DHT.humidity;
    //read temperature 
    Serial.print(h);
    float f = DHT.temperature;
    if (isnan(h) || isnan(f)) {
      lcd.setCursor(0,0);
      lcd.print("ERROR");
      return;
    }
    d = distanza();
    if(d > 40){
      Serial.print(">40");
    }else{
      Serial.print("<=40");
    }
    ThingSpeak.setField(1,f);
    ThingSpeak.setField(2,h);
    ThingSpeak.setField(3,d);
    ThingSpeak.writeFields(myChannelNumber, myWriteAPIKey);
  
    Serial.print("Humidity: ");
    Serial.print(h);
    Serial.print(" %\t ");
    Serial.print("Temperature: ");
    Serial.print(f);
  
    lcd.setCursor(0,0);
    lcd.print("Hum:");
    lcd.setCursor(7,0);
    lcd.print("Temp:");
    lcd.setCursor(0,1);
    lcd.print(h);
    lcd.setCursor(7,1);
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
