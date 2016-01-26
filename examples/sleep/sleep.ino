void setup() {
  // We don't need to set anything up here
}

void loop() {
  // Check if LED is on
  if (Bean.getLedGreen() == 0) {
    // LED is off. Turn LED on and sleep for 1 second
    Bean.setLed(0, 255, 0);
    Bean.sleep(1000);
  } else {
    // LED is on. Turn LED off and sleep for 4 seconds
    Bean.setLed(0, 255, 0);
    Bean.sleep(4000);
  }
}
