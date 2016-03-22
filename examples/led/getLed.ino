void setup() {
  // Serial port is initialized automatically; we don't have to do anything
}

void loop() {
  LedReading rgbSetting = Bean.getLed();
  Serial.print("  Red: ");
  Serial.println(rgbSetting.red);
  Serial.print("Green: ");
  Serial.println(rgbSetting.green);
  Serial.print(" Blue: ");
  Serial.println(rgbSetting.blue);
  Bean.sleep(2000);
}
