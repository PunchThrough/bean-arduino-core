void setup() {
  // We don't need to set anything up here
}

void loop() {
  bool advertising = Bean.getAdvertisingState();

  if (advertising) {
    // Bean is currently advertising.
    // Disable advertising and set the LED to red.
    Bean.setLed(255, 0, 0);
  } else {
    // Bean is not advertising.
    // Enable advertising and set the LED to green.
    Bean.setLed(0, 255, 0);
  }

  Bean.sleep(15000);  // Sleep for 15 seconds
}
