void setup() {
  // Configure digital pin 0 as an input in pullup mode
  pinMode(0, INPUT_PULLUP);
}

void loop() {
  // Read digital pin 0
  if (!digitalRead(0)) {
    // D0 is LOW; disable advertising for 10 seconds
    Bean.enableAdvertising(false, 10000);
  }

  Bean.sleep(1000);  // Sleep for 1 second
}
