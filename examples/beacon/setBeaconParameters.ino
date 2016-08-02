void setup() {
  // Enable iBeacon and add it to advertising rotation
  Bean.enableiBeacon();
  /* Configure beacon with:
  *  ----------------------
  *  UUID: A495BEEF-C5B1-4B44-B512-1370F02D74DE
  *  Major ID: 0 (Any unsigned int 0-66535)
  *  Minor ID: 65535 (Any unsigned int 0-66535)
  */
  Bean.setBeaconParameters(A495BEEFC5B14B44B5121370F02D74DE, 0, 66535);
}
void loop() {
  // Sleep forever while transmitting iBeacon location data
  Bean.sleep(0xffffffff);
}