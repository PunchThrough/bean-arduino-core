void setup() {
  // Enable 6-digit pairing PIN code (default is 000000)
  Bean.enablePairingPin(true);
  // Set pairing PIN to 123456
  Bean.setPairingPin(123456);
  // Pairing PIN will be required when future connections are established
}
void loop() {
  // Sleep forever
  Bean.sleep(0xffffffff);
}
