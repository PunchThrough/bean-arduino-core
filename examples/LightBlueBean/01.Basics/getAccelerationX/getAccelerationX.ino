
/*
Led from Accelerometer.
 
Set the red LED color in accordance with absolute values of acceleration.
*/
 
void setup() {
// initialize serial communication at 57600 bits per second:
Serial.begin(57600);
}
 
// the loop routine runs over and over again forever:
void loop() {
 
int16_t accelX;
accelX = Bean.getAccelerationX();
 
int16_t r = abs(accelX) / 4;
 
Bean.setLed((uint8_t)r, 0, 0);
 
delay(50);
}
