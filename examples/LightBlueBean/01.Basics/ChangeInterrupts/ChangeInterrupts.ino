#include <PinChangeInt.h>

/* 
  This sketch shows how to use low-power interrupts to detect when the Bean's digital inputs have changed. 
  
  To use the sketch, conect a wire between the Bean's ground pin and one of it's 6 digital pins. 
  Pins 0, 1, and 2 will enabled colors in the LED, while the other pins disable colors. 
  
  Notes:
    - This is not a low-power sketch because of LED use
    - A Bean with a low battery might show a faint blue and green LED color
  
  This example code is in the public domain.
*/

volatile uint8_t red = 0;
volatile uint8_t green = 0;
volatile uint8_t blue = 0;

// Interrupts are meant to be fast, avoid doing Bean.* operations directly within ISRs
//   Set a flag and perform the desired operation within the superloop
void zeroChangeDetected( void )
{
  red = 128;
}

void oneChangeDetected( void )
{
  green = 128;
}

void twoChangeDetected( void )
{
  blue = 128;
}

void threeChangeDetected( void )
{
  red = 0;
}

void fourChangeDetected( void )
{
  green = 0;
}

void fiveChangeDetected( void )
{
  blue = 0;
}

void setup() {
  pinMode(0, INPUT_PULLUP);
  pinMode(1, INPUT_PULLUP);
  pinMode(2, INPUT_PULLUP);
  pinMode(3, INPUT_PULLUP);
  pinMode(4, INPUT_PULLUP);
  pinMode(5, INPUT_PULLUP);
  attachPinChangeInterrupt(0, zeroChangeDetected, CHANGE);
  attachPinChangeInterrupt(1, oneChangeDetected, CHANGE);
  attachPinChangeInterrupt(2, twoChangeDetected, CHANGE);
  attachPinChangeInterrupt(3, threeChangeDetected, CHANGE);
  attachPinChangeInterrupt(4, fourChangeDetected, CHANGE);
  attachPinChangeInterrupt(5, fiveChangeDetected, CHANGE);
}

void loop() {
  // Set the Bean's LED based upon RGBs changed by interrupts
  Bean.setLed(red, green, blue);
  Bean.sleep(1000);
}

