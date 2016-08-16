// Create boolean to run loop persistently
bool motionDetected = false;
void setup() {
  // Enable low-g motion events to be detected
  Bean.enableMotionEvent(ANY_MOTION_EVENT);
  // (The next two lines are equivalent to "uint8_t value = Bean.getAccelerometerPowerMode()")
  uint8_t value;
  Bean.accelRegisterRead(REG_POWER_MODE_X11, 1, &value);
  if (value != VALUE_NORMAL_MODE) {
    // Set accelerometer power mode to normal power mode if it isn't already
    // (This is the same as "Bean.setAccelerometerPowerMode(VALUE_NORMAL_MODE)")
    Bean.accelRegisterWrite(REG_POWER_MODE_X11, VALUE_NORMAL_MODE);
  }
}
void loop() {
  // Check for low-g movement
  if (Bean.checkMotionEvent(ANY_MOTION_EVENT) && !motionDetected) {
    motionDetected = true;
  } else if (!Bean.checkMotionEvent(ANY_MOTION_EVENT) && !motionDetected) {
    Bean.sleep(2000);  // Sleep for 2 seconds before rechecking
  }
  if (motionDetected) {
    // If motion is detected, blue/red lights will flash continuously
    Bean.setLed(255, 0, 0);
    Bean.sleep(500);
    Bean.setLed(0, 0, 255);
    Bean.sleep(500);
    Bean.setLed(255, 0, 0);
    Bean.sleep(500);
    Bean.setLed(0, 0, 255);
    Bean.sleep(500);
    Bean.setLed(255, 0, 0);
    Bean.sleep(500);
    Bean.setLed(0, 0, 255);
    Bean.sleep(500);
    Serial.print("Motion has been detected; ");
    Serial.println();
  }
}
