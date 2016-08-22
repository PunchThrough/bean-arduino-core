void setup() {
  Serial.begin(57600);
}

void loop() {
  Bean.getAcceleration();
  Bean.setLed(0, 255, 255);
}
