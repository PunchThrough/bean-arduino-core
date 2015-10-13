void setup() {
  // We don't need to set anything up here
}

void loop() {
  // Create a 2-byte buffer to hold our analog value when we split it into bytes
  uint8_t buffer[2];

  // Read 10-bit values from A0 and A1
  uint16_t an0 = analogRead(A0);
  uint16_t an1 = analogRead(A1);

  // Split A0's 10-bit value into two bytes and store in the buffer:
  // [<least significant 8 bits>, <most significant two bits>]
  buffer[0] = an0 & 0xFF;
  buffer[1] = an0 >> 8;

  // Store those two bytes in scratch characteristic 1
  Bean.setScratchData(1, buffer, 2);

  // Do the same as above: split A1...
  buffer[0] = an1 & 0xFF;
  buffer[1] = an1 >> 8;

  // ...and store in scratch characteristic 2
  Bean.setScratchData(2, buffer, 2);

  // Sleep for 3 seconds
  // If you're connected to this Bean in LightBlue,
  // you should see Notify events firing every 3 seconds
  Bean.sleep(10000);
}
