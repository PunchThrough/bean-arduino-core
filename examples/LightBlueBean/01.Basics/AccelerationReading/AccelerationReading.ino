/*
Led from Accelerometer.

Set the LED colors in accordance with absolute values of acceleration.
*/

void setup() {
// initialize serial communication at 57600 bits per second:
Serial.begin(57600);
}

// the loop routine runs over and over again forever:
void loop() {
AccelerationReading accel = {0, 0, 0};

accel = Bean.getAcceleration();

uint16_t r = (abs(accel.xAxis)) / 4;
uint16_t g = (abs(accel.yAxis)) / 4;
uint16_t b = (abs(accel.zAxis)) / 4;

Bean.setLed((uint8_t)r,(uint8_t)g,(uint8_t)b);

delay(50);
}
