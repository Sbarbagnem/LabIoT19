#include <Wire.h>
#include "rgb_lcd.h"
rgb_lcd lcd;

#include <dht11.h>
dht11 DHT;
#define DHT11_PIN 4

void setup(){
  Serial.begin(9600);
  lcd.begin(16, 2);
  Serial.print("LIBRARY VERSION: ");
  Serial.println(DHT11LIB_VERSION);
  Serial.println();
  Serial.println("Type,\tstatus,\tHumidity (%),\tTemperature (C)");
}

void loop(){
  int chk;
  chk = DHT.read(DHT11_PIN);    // READ DATA
  delay(500);
  // set the cursor to column 0, line 1
  // (note: line 1 is the second row, since counting begins with 0):
  // read humidity
  float h = DHT.humidity;
  //read temperature in Fahrenheit
  float f = DHT.temperature;
  if (isnan(h) || isnan(f)) {
    lcd.setCursor(0,0);
    lcd.print("ERROR");
    return;
  }
  lcd.setCursor(0,0);
  lcd.print("Hum:");
  lcd.setCursor(7,0);
  lcd.print("Temp:");
  lcd.setCursor(0,1);
  lcd.print(h);
  lcd.setCursor(7,1);
  lcd.print(f); 

}
