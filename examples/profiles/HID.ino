/*
 * This is an example sketch showing how to use HID (Human Interface Device).
 * HID allows Bean to act as a mouse, keyboard, or media control device.
 *
 * In this example, Bean acts as an air mouse. Tilting Bean will move the mouse
 * pointer around the screen.
 *
 * Sending '1' over Virtual Serial tells Bean to type "Hello, world!" on the
 * keyboard, and sending '2' tells Bean to send the Play media command.
 *
 * After enabling HID for the first time, power cycle the Bean. You must enter
 * a pairing code of 00000 to connect to Bean.
 *
 * Important: When disconnecting from a device, you must manually forget the
 * Bean in the system Bluetooth menu as well as Bean Loader.
 */

void setup() {
  BeanHid.enable();
  // Serial port is initialized automatically; we don't have to do anything
}

void loop() {
  AccelerationReading accel = Bean.getAcceleration();

  int16_t x = accel.xAxis;
  int16_t y = accel.yAxis;
  int16_t z = accel.zAxis;

  // Add a deadzone. If Bean is reasonabally flat, don't move the mouse pointer
  if (abs(x) < 20) {
    x = 0;
  }
  if (abs(y) < 20) {
    y = 0;
  }

  // we want to map the movements to reasonable values.
  // This constrains the accelerometer to -20 to 20.
  int16_t mousex = map(x, -60, 60, -20, 20);
  int16_t mousey = map(y, -60, 60, 20, -20);

  BeanHid.moveMouse(mousex, mousey);

  if (Serial.available()) {
    byte read = Serial.read();
    if (read == '1') {
      // send the ASCII number 1 to type a message
      BeanHid.sendKeys("Hello, world!");
    } else if (read == '2') {
      // send the ASCII number 2 to use a media command
      BeanHid.sendMediaControl(PLAY);
    }
  }
}
