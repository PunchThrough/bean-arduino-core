void setup() {
  // Enable low-g motion events to be detected
  Bean.enableMotionEvent(LOW_G_EVENT);
  // Create boolean to run loop persistently
  bool motionDetected = false;
  bool motionFlag;
  // Make sure accelerometer is in low power mode to save battery
  // (The next two lines are equivalent to "uint8_t value = Bean.getAccelerometerPowerMode()")
  uint8_t value;
  Serial.accelRegisterRead(REG_POWER_MODE_X11, 1, &value);
  if (value != VALUE_LOW_POWER_1S){
  	// Set accelerometer power mode to low power consumption 
    // (This is the same as "Bean.setAccelerometerPowerMode(VALUE_LOW_POWER_1S)")
  	Bean.accelRegisterWrite(REG_POWER_MODE_X11, VALUE_LOW_POWER_1S);
  }
}
void loop() {
  if (motionDetected){
    // If motion is detected, blue/red lights will flash continuously
  	Bean.setLed(255, 0, 0);
    Bean.sleep(500)
    Bean.setLed(0, 0, 255);
    Bean.sleep(500)
    Bean.setLed(255, 0, 0);
    Bean.sleep(500)
    Bean.setLed(0, 0, 255);
    Bean.sleep(500)
    Bean.setLed(255, 0, 0);
    Bean.sleep(500)
    Bean.setLed(0, 0, 255);
    Bean.sleep(500)
    Serial.print("Motion has been detected; ");
    Serial.println();
  }
  // Check for low-g movement
  if (Bean.checkMotionEvent(LOW_G_EVENT) && !motionDetected){
    motionDetected = true;
  } else if (!Bean.checkMotionEvent(LOW_G_EVENT) && !motionDetected){
    Bean.sleep(2000);  // Sleep for 2 seconds before rechecking
  }
  
}