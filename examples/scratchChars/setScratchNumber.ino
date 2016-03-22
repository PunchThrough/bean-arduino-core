void setup() {
  // Serial port is initialized automatically; we don't have to do anything
}

void loop() {
  // Write a 32-bit number to scratch characteristic 1
  // Connect to Bean with LightBlue to view the hex bytes
  Bean.setScratchNumber(1, millis());
  // Then wait a bit
  Bean.sleep(250);

  // Read the number back and print it via serial
  uint32_t number = Bean.readScratchNumber(1);
  Serial.print("Number: ");
  Serial.println(number);

  // Then wait a bit and try again
  Bean.sleep(750);
}
