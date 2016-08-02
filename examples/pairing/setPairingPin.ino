void setup() {
  // Enable 6-digit pairing PIN code (default is 000000)
  Bean.enablePairingPin(true);
  // Set pairing PIN to 123456
  Bean.setPairingPin(123456);
  // Pairing PIN will be required when future connections are established
}
void loop() {
  // Code for something secretive
  Bean.sleep(10000);  // Sleep for 10 seconds
}