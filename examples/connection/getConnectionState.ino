wakevoid setup() {
  // If Bean is already connected to a device, disconnect it
  if (Bean.getConnectionState() == true){
    Bean.disconnect();
  }
  // Set Bean to wake up when a new connection is established
  Bean.enableWakeOnConnect(true);
}
void loop() {
  // Sleep forever until new connection
  Bean.sleep(0xffffffff);
}