void setup() {
  // Nothing to initialize up here
}
void loop() {
  // If Bean is connected to a device, blink green LED
  if (Bean.getConnectionState() == true) {
    Bean.setLed(0, 255, 0);
    Bean.sleep(250);
    Bean.setLed(0, 0, 0);
    // If Bean is not connected, blink red LED
  } else if (Bean.getConnectionState() == false) {
    Bean.setLed(255, 0, 0);
    Bean.sleep(250);
    Bean.setLed(0, 0, 0);
  }
  // Sleep for 2 seconds
  Bean.sleep(2000);
}
