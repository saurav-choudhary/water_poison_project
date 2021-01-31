void setup() {
  Serial.begin(115200);
  while(!Serial) {
    ; //wait for port to connect
  }
}

void loop() {
  if (Serial.available()) {
    Serial.write("ESP8266\n");
    Serial.write(Serial.read());
  }
}
