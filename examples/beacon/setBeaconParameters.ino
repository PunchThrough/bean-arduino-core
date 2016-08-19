void setup() {
  /* Configure beacon with:
  *  ----------------------
  *  UUID: A495-BEEF-C5B1-4B44-B512-1370F02D74DE
  *  Major ID: 0 (Any unsigned int 0-66535)
  *  Minor ID: 65535 (Any unsigned int 0-66535)
  */
  Bean.setBeaconParameters(0xBEEF, 0, 999);
  // Enable iBeacon
  Bean.setBeaconEnable(true);
}
void loop() {
  // Blink LED to verify the sketch is working
  Bean.setLed(0, 0, 100);
  Bean.sleep(100);
  Bean.setLed(0, 0, 0);
  Bean.sleep(5000);
}
