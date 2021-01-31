#include "DHT.h"
#include <SoftwareSerial.h>
#define DHTPIN 2

#define DHTTYPE DHT11
//#define DHTTYPE DHT22 // DHT 22 (AM2302), AM2321
//#define DHTTYPE DHT21 // DHT 21 (AM2301)

SoftwareSerial espSerial(18, 19);
DHT dht(DHTPIN, DHTTYPE);
String str;

void setup() {
  Serial.begin(115200);
  espSerial.begin(115200);
  dht.begin();
  delay(2000);
}

void loop() {
  float h = dht.readHumidity();
  float t = dht.readTemperature();

  Serial.print("Humidity: ");
  Serial.print(h);
  Serial.print("%");
  Serial.print("\nTemp: ");
  Serial.print(t);
  Serial.println("C\n");
  
  str = String("Data from Arduino\n") + String("Humidity: ") + String(h) + String("\nTemp: ") + String(t) + String("C\n");
  espSerial.println(str);
  delay(1000);
}
