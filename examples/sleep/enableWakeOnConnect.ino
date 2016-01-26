void setup() {
  // Configure Bean to wake up when a client connects
  Bean.enableWakeOnConnect(true);
}

// Loop runs once at program start.
// When a client connects, Bean wakes up and runs loop again.
// Loop continues to run as long as a client is connected.

void loop() {
  // Check if client has disconnected
  bool connected = Bean.getConnectionState();

  if (connected) {
    // Client connected: set LED to green
    Bean.setLed(0, 255, 0);

  } else {
    // Client disconnected: turn off LED
    Bean.setLed(0, 0, 0);

    // This sketch runs only when a client is connected.
    // This might be useful for interactive installations, where you don't want
    // Bean to spend any power unless someone is actively connected.

    // Sleep for a long time to conserve power.
    // 0xFFFFFFFF = 4,294,967,295 milliseconds = 49 days
    Bean.sleep(0xFFFFFFFF);
  }
}
