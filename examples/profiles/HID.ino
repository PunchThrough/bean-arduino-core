/*
 * This is an example sketch for how to use HID.  The user must enter a pairing code of 00000 to connect.
 * HID allows the bean to act as a mouse, keyboard, or media control device.
 * The bean will move a mouse pointer around the screen according to how it is tilted.
 * The user can send commands over the serial terminal to see a demonstration of different actions.
 * NOTE: Mouse clicks may not work on OSX at the moment (March 21, 2016)
 *
 *
 * Important, when disconnecting from a device, you must manually forget the Bean in the system 
 * bluetooth menu as well as the loader
 * After enabling HID for the first time, power cycle the Bean.
 */

void setup() {
    Serial.begin();
    BeanHid.enable();
}

void loop() {
    AccelerationReading accel = Bean.getAcceleration();

    int16_t x = ((accel.xAxis));
    int16_t y = ((accel.yAxis));
    int16_t z = ((accel.zAxis));

    // if the Bean is reasonabally flat, don't move the mouse pointer
    if (abs(x) < 20) {  // deadzone
        x = 0;
    }
    if (abs(y) < 20) {  // deadzone
        y = 0;
    }

    // we want to map the movements to reasonable values.
    // This constrains the accelerometer to -20 to 20.
    int16_t mousex = map(x, -60, 60, -20, 20);
    int16_t mousey = map(y, -60, 60, 20, -20);

    BeanHid.moveMouse(mousex, mousey);


    if (Serial.available()) {
        byte read = Serial.read();
        if (read == '1') {  // send the number 1 to type a message
            BeanHid.sendKeys("Hello, world!");
        } else if (read == '2') {  // send the number 2 to use a media command
            BeanHid.sendMediaControl(PLAY);
        }
    }
}
