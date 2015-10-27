void setup() {
  // Set up serial communication
  Serial.begin(57600);
}

void loop() {
  // Retrieve the name of this Bean
  const char *name = Bean.getBeanName();

  // And print it
  Serial.print("My name is ");
  Serial.println(name);

  // Then sleep for 2 seconds
  Bean.sleep(2000);
}
