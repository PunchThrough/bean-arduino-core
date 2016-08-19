void setup() {
  // Set enableConfigSave to true to save settings temporarily
  Bean.enableConfigSave(false);
  // Enable some settings which will be temporary now
  Bean.enableAdvertising(true);
  Bean.enableCustom();
  Bean.enableiBeacon();
  // These settings will be reset to default upon rebooting the Bean
}
void loop() {
  // Do anything here with new temporary settings
}
