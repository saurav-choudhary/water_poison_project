#include <OneWire.h>
#include <DallasTemperature.h>
#include <ArduinoJson.h>

OneWire oneWire(2);
DallasTemperature temp_sensor(&oneWire);

float calibration_value = 27.84;
int phVal = 0;
unsigned long int avgval;
int buffer_arr[10], temp;

void setup()
{
  Serial.begin(9600);
  temp_sensor.begin();
}

StaticJsonBuffer<1000> jsonBuffer;
JsonObject& root = jsonBuffer.createObject();

void loop() {
  for (int i = 0; i < 10; i++)
  {
    buffer_arr[i] = analogRead(A0);
    delay(30);
  }
  for (int i = 0; i < 9; i++)
  {
    for (int j = i + 1; j < 10; j++)
    {
      if (buffer_arr[i] > buffer_arr[j])
      {
        temp = buffer_arr[i];
        buffer_arr[i] = buffer_arr[j];
        buffer_arr[j] = temp;
      }
    }
  }
  avgval = 0;
  for (int i = 2; i < 8; i++)
    avgval += buffer_arr[i];
  float volt = (float)avgval * 5.0 / 1024 / 6;
  float ph_act = -5.70 * volt + calibration_value;
  temp_sensor.requestTemperatures();
  int moisture_analog=analogRead(A1);
  int moist_act=map(moisture_analog,0,1023,100,0);
  root["ph"] = ph_act;
  root["temp"] = temp_sensor.getTempCByIndex(0);
  root["moist"] = moist_act;
  root.printTo(Serial);
  Serial.println("");
}
 
 
NodeMCU Code:
#include<ESP8266WiFi.h>
#include<WiFiClient.h>
#include<ESP8266WebServer.h>
#include <ArduinoJson.h>
const char* ssid = "wifi-ssid";
const char* password = "wifi-pass";
ESP8266WebServer server(80);
String page = "";
int data1, data2, data3;
void setup()
{
  Serial.begin(9600);
  WiFi.begin(ssid, password);
while (WiFi.status() != WL_CONNECTED)
  {
  delay(500);
  Serial.print(".");
  }
  Serial.println(WiFi.localIP());
  server.on("/", []()
  {
    page = "<html><head><title>IoT Design</title></head><style type=\"text/css\">";
    page += "table{border-collapse: collapse;}th {background-color:  green ;color: white;}table,td {border: 4px solid black;font-size: x-large;";
    page += "text-align:center;border-style: groove;border-color: rgb(255,0,0);}</style><body><center>";
    page += "<h1>Smart Aquaculture Monitoring using IoT</h1><br><br><table style=\"width: 1200px;height: 450px;\"><tr>";
    page += "<th>Parameters</th><th>Value</th><th>Units</th></tr><tr><td>PH Value</td><td>"+String(data1)+"</td><td>N/A</td></tr>";
    page += "<tr><td>Temperature</td><td>"+String(data2)+"</td><td>Centigrade</td></tr><tr><td>Moisture</td><td>"+String(data3)+"</td><td>%</td>";
    page += "<meta http-equiv=\"refresh\" content=\"3\">";
    server.send(200, "text/html", page);
  });
  server.begin();
}
void loop()
{
  StaticJsonBuffer<1000> jsonBuffer;
  JsonObject& root = jsonBuffer.parseObject(Serial);
  if (root == JsonObject::invalid())
  {
    return;
    Serial.println("invalid");
  }
  data1 = root["ph"];
  data2 = root["temp"];
  data3 = root["moist"];
  Serial.println(data1);
  Serial.println(data2);
  Serial.println(data3);
  server.handleClient();
}
