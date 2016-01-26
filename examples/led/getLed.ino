void setup() {
  Serial.begin(57600);  // Initialize serial port
}

void loop() {
  LedReading rgbSetting = Bean.getLed();;
  Serial.print("  Red: ");
  Serial.println(rgbSetting.red);
  Serial.print("Green: ");
  Serial.println(rgbSetting.green);
  Serial.print(" Blue: ");
  Serial.println(rgbSetting.blue);
  Bean.sleep(2000);
}
