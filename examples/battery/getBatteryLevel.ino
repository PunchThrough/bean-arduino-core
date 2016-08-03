void setup() {
  // Serial port is initialized automatically; we don't have to do anything
}
void loop() {
  int batteryPercentage = Bean.getBatteryLevel();  // Between 0 and 100
  int batteryVoltage = Bean.getBatteryVoltage();   // Between 191 and 353 (1.91V-3.53V)
  Serial.print("The remaining battery life is: %");
  Serial.println(batteryPercentage);
  blinkLedBasedOnBatteryLevel(batteryPercentage);
  Serial.print("The current battery voltage is: ");
  Serial.print(batteryVoltage / 100);
  Serial.println("V");
  Bean.sleep(5000);  // Sleep for 5 seconds before repeating
}
void blinkLedBasedOnBatteryLevel(int batteryPercentage) {
  if (batteryPercentage > 80) {
    Bean.setLed(0, 255, 0);
  } else if (batteryPercentage > 60) {
    Bean.setLed(255, 255, 0);
  } else if (batteryPercentage > 40) {
    Bean.setLed(255, 150, 0);
  } else if (batteryPercentage > 20) {
    Bean.setLed(255, 75, 0);
  } else {
    Bean.setLed(255, 0, 15);
  }
  Bean.sleep(100);
  Bean.setLed(0, 0, 0);
}
