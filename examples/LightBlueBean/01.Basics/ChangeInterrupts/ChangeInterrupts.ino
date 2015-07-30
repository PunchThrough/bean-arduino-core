#include <PinChangeInt.h>

/* 
  This sketch uses the Bean library to trigger interrupts when changes are detected on pins 0, 1, and 2. 
  
  Notes:
    - This is not a low-power sketch 
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
  red += 16;
}

void oneChangeDetected( void )
{
  green += 16;
}

void twoChangeDetected( void )
{
  blue += 16;
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
  pinMode(0, INPUT);
  pinMode(1, INPUT);
  pinMode(2, INPUT);
  pinMode(3, INPUT);
  pinMode(4, INPUT);
  pinMode(5, INPUT);
  PCintPort::attachInterrupt(0, zeroChangeDetected, CHANGE);
  PCintPort::attachInterrupt(1, oneChangeDetected, CHANGE);
  PCintPort::attachInterrupt(2, twoChangeDetected, CHANGE);
  PCintPort::attachInterrupt(3, threeChangeDetected, CHANGE);
  PCintPort::attachInterrupt(4, fourChangeDetected, CHANGE);
  PCintPort::attachInterrupt(5, fiveChangeDetected, CHANGE);
  Bean.setLed(0xFF, 0xFF, 0xFF);  // Flash to show the sketch is running
  Bean.sleep(250);
}

void loop() {
  // Set the Bean's LED based upon RGBs changed by interrupts
  //   Eventually, overflow will turn the colors off
  Bean.setLed(red, green, blue);
  Bean.sleep(100);
}
