void setup() {
  Serial.begin(115200);
  while(!Serial) {
    ; //wait for port to connect
  }
}

void loop() {
  if (Serial.available()) {
    Serial.write(Serial.read());
  }
}