void setup() {
  // Configure Bean to wake up when a client connects
  Bean.enableWakeOnConnect(true);
}

// Loop runs once when a client connects and continues running as long as a
// client is connected
void loop() {
  // Check if client has disconnected
  bool connected = Bean.getConnectionState();
  if (connected) {
    // Client connected: set LED to green
    Bean.setLed(0, 255, 0);
  } else {
    // Client disconnected: turn off LED and sleep for a long time
    Bean.setLed(0, 0, 0);
    // 0xFFFFFFFF = 4,294,967,295 milliseconds = 49 days
    Bean.sleep(0xFFFFFFFF);
  }
}
