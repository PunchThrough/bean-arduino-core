void setup() {
  // Keep Bean awake to reduce latency between certain events
  Bean.keepAwake(true);
  pinMode(0, INPUT_PULLUP);
}
void loop() {
  // Read digital pin 0
  if (!digitalRead(0)) {
    // D0 is LOW; check to see if BLE connected to any devices
    bool connectionState = Bean.getConnectionState();
    Serial.println(connectionState);  // True if connected to a device, false if not
  }
  Bean.sleep(1000);  // Sleep for 1 second
}
