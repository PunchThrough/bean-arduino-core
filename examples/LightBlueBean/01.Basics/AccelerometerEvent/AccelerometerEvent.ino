/* 
  This sketch demonstrates the Bean's ability to detect motion events.

  In this demo we assign a unique LED color with each motion event.
  The Bean's active LED color indicates what motion event it's waiting to detect. 
  After detecting the event, the sketch will cycle to the next color and wait for the next event. 
  
  We start by waiting for "double tap" indicated by a red LED color
  
  - Double tap          (Red) 
  - Single tap          (Green)
  - Any motion detected (Blue)
  - Orientation change  (Orange)
  - High-g motion       (Purple)
  - Low-g (freefall)    (Turquoise)
  - Flat orientation    (Off)
  
  Notes:
    - This is not a low-power sketch because we are using the LED extensively.
      Without LED use, this sketch is considered low-power. 
    - Bean.sleep() is interrupted by the action of an enabled event. 
    - Calling checkMotionEvent(someEvent) will return 'true' only if "someEvent" was 
      detected in the time since checkMotionEvent(someEvent) was last called.
  
  This example code is in the public domain.
*/

typedef struct{
  uint8_t event;
  uint8_t red;
  uint8_t green;
  uint8_t blue;
}MotionEventState;

int index = 0;
MotionEventState states[] = {
  {DOUBLE_TAP_EVENT, 128,    0,    0 },          // Red 
  {SINGLE_TAP_EVENT,   0,  128,    0 },          // Green
  {ANY_MOTION_EVENT,   0,    0,  128 },          // Blue
  {ORIENT_EVENT,      64,   64,    0 },          // Orange
  {HIGH_G_EVENT,      64,    0,   64 },          // Purple
  {LOW_G_EVENT,        0,   64,   64 },          // Turquoise
  {FLAT_EVENT,         0,    0,    0 }           // Off
};


void setup() {
}

void loop() {
  // Listen for this event
  Bean.enableMotionEvent( states[index].event );

  // Set the LED state to indicate which event the Bean is waiting for
  Bean.setLed(states[index].red, states[index].green, states[index].blue);

  // Wait for the motion event in a low-power mode
  while( Bean.checkMotionEvent( states[index].event ) == false )
  {
    // Bean.sleep is interrupted by an enabled acclerometer event
    Bean.sleep(10000);
  }

  // Disable this event
  Bean.disableMotionEvents();
  
  // Move on to the next event
  index = (index + 1) % 7;
}
