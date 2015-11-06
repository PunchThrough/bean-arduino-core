void setup() {
  // Disable writing to NVRAM to preserve its lifespan,
  // since we are setting a new Bean name every 15 seconds
  Bean.enableConfigSave(false);
}

void loop() {
  // Get the temperature
  int temperature = Bean.getTemperature();

  // Build a new name for Bean using the temperature
  String name = "Temp is ";
  name += temperature;
  name += " C";

  // Set Bean's new name
  Bean.setBeanName(name);

  // Sleep for 15 seconds, then read the temperature again
  Bean.sleep(15000);
}
