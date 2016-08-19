void setup() {
  // Serial port is initialized automatically; we don't have to do anything
}
void loop() {
  // Get temperature reading
  int temperature = Bean.getTemperature();
  // Set LED color based on temperature of the Bean
  if (temperature > 80) {
    Bean.setLed(255, 0, 0);  // Set LED red
  } else if (temperature > 60) {
    Bean.setLed(255, 150, 0);  // Set LED orange
  } else if (temperature > 40) {
    Bean.setLed(255, 255, 0);  // Set LED yellow
  } else if (temperature > 20) {
    Bean.setLed(0, 255, 0);  // Set LED green
  } else {
    Bean.setLed(0, 0, 255);  // Set LED blue
  }
  Bean.sleep(5000);  // Sleep for 5 seconds before rechecking temperature
}
