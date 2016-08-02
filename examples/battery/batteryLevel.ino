void setup() {
  // Serial port is initialized automatically; we don't have to do anything
}
void loop() {
  int batteryPercentage = Bean.getBatteryLevel();  // Between 0 and 100
  int batteryVoltage = Bean.getBatteryVoltage();  // Between 191 and 353 (1.91V-3.53V)
  Serial.print("The remaining battery life is: %");
  Serial.println(batteryPercentage);
  Serial.print("The current battery voltage is: ");
  Serial.print(batteryVoltage / 100);
  Serial.println("V");
  Bean.sleep(5000);  // Sleep for 5 seconds before repeating
}