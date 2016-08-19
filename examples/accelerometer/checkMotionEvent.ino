// Create boolean to run loop persistently
bool motionDetected = false;
void setup() {
  // Enable low-g motion events to be detected
  Bean.enableMotionEvent(ANY_MOTION_EVENT);
  // Make sure accelerometer is in normal power mode
  if (Bean.getAccelerometerPowerMode() != VALUE_NORMAL_MODE) {
    // Set accelerometer power mode to normal
    Bean.setAccelerometerPowerMode(VALUE_NORMAL_MODE);
  }
  // Initially set LED to blank
  Bean.setLed(0, 0, 0);
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
    Bean.sleep(100);
    Bean.setLed(0, 0, 255);
    Bean.sleep(100);
    Bean.setLed(255, 0, 0);
    Bean.sleep(100);
    Bean.setLed(0, 0, 255);
    Bean.sleep(100);
    Bean.setLed(255, 0, 0);
    Bean.sleep(100);
    Bean.setLed(0, 0, 255);
  }
}
