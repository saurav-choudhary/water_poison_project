#include "DHT.h"
#include <SoftwareSerial.h>
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#define ONE_WIRE_BUS 2

OneWire oneWire(ONE_WIRE_BUS); 

DallasTemperature sensors(&oneWire);

LiquidCrystal_I2C lcd(0x27, 16, 2);

int sensorPin = A0;  
int sensorValue = 0;  
int percent = 0;
int resval = 0;
int respin = A1;

#define DHTPIN 2

#define DHTTYPE DHT11
//#define DHTTYPE DHT22 // DHT 22 (AM2302), AM2321
//#define DHTTYPE DHT21 // DHT 21 (AM2301)

SoftwareSerial espSerial(18, 19);
DHT dht(DHTPIN, DHTTYPE);
String str, lcd_out;

void setup() {
  Serial.begin(115200);
  espSerial.begin(115200);
  dht.begin();
  lcd.begin();
  lcd.backlight();
  sensors.begin();
  delay(2000);
}

void loop() {
  sensorValue = analogRead(sensorPin);
//  percent = convertToPercent(sensorValue);
  float h = dht.readHumidity();
  float t = dht.readTemperature();

  Serial.print("Humidity: ");
  Serial.print(h);
  Serial.print("%");
  Serial.print("\nTemp: ");
  Serial.print(t);
  Serial.println("C\n");
  
  str = String("Data from Arduino\n") + String("Humidity: ") + String(h) + String("\nTemp: ") + String(t) + String("C\n");
  lcd_out = String("H:") + String(h) + String(" T:") + String(t);
  espSerial.println(str);
  delay(1000);
  lcd.print(lcd_out);
  delay(1000);
  lcd.clear();
}
