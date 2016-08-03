void setup() {
  // Enable motion events to see how they're calibrated
  Bean.enableMotionEvent(LOW_G_EVENT);
  Bean.enableMotionEvent(HIGH_G_EVENT);
  // Configure accelerometer to sense ±16g if it's not set already
  if (Bean.getAccelerationRange() != 16) {
    Bean.setAccelerationRange(16);
  }
}
void loop() {
  // Sleep forever while transmitting iBeacon location data
  if (Bean.checkMotionEvent(HIGH_G_EVENT)) {
    Serial.print("High-g event detected: ");
    printAcceleration(Bean.getAcceleration());
    // Blink green LED
    Bean.setLed(0, 255, 0);
    Bean.sleep(1000);
  } else if (Bean.checkMotionEvent(LOW_G_EVENT)) {
    Serial.print("Low-g event detected: ");
    printAcceleration(Bean.getAcceleration());
    // Blink yellow LED
    Bean.setLed(255, 255, 0);
    Bean.sleep(1000);
  } else {
    Serial.print("No movement detected: ");
    printAcceleration(Bean.getAcceleration());
    // Blink red LED
    Bean.setLed(255, 0, 0);
    Bean.sleep(1000);
  }
  Bean.sleep(1000);  // Sleep and recheck every second
}
void printAcceleration(AccelerationReading acceleration) {
  // Print axes of acceleration in a nice-looking way
  Serial.print("(x= ");
  Serial.print(acceleration.xAxis);  // Same as getAccelerationX()
  Serial.print(", y= ");
  Serial.print(acceleration.yAxis);  // Same as getAccelerationY()
  Serial.print(", z= ");
  Serial.print(acceleration.zAxis);  // Same as getAccelerationZ()
  Serial.println(")");
}
