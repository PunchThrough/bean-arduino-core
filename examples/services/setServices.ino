void setup() {
  // Get and print the current Bluetooth services
  BluetoothServices originalServices = Bean.getServices();
  printServices(originalServices);
  // Reset the services to default
  Bean.resetServices();
  // Reset the Bluetooth module itself
  Bean.restartBluetooth();
  // Wait for a few moments for Bluetooth to come back up...
  Bean.sleep(5000);
  // Set services to include standard advertising (default)
  BluetoothServices newServices;
  newServices.standard = 1;
  Bean.setServices(newServices);
  // Print out new service configuration
  printServices(newServices);
}
void loop() {
  // Do anything here
}
void printServices(BluetoothServices services) {
  Serial.println("Bluetooth services:");
  Serial.print("Standard: ");
  Serial.println(services.standard);
  Serial.print("HID: ");
  Serial.println(services.hid);
  Serial.print("MIDI: ");
  Serial.println(services.midi);
  Serial.print("ANCS: ");
  Serial.println(services.ancs);
  Serial.print("Observer: ");
  Serial.println(services.observer);
  Serial.print("iBeacon: ");
  Serial.println(services.ibeacon);
  Serial.print("Custom: ");
  Serial.println(services.custom);
}
