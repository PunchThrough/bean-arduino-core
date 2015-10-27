void setup() {
  // We don't need to set anything up here
}

void loop() {
  // Get the temperature
  int temperature = Bean.getTemperature();

  // Build a new name for Bean using the temperature
  String name = "Temp is ";
  name += temp;
  name += " C";

  // Set Bean's new name
  Bean.setBeanName(name);

  // Sleep for 15 seconds, then read the temperature again
  Bean.sleep(15000);
}
