/*
  Blink
  Turns on an LED on for one second, then off for one second, repeatedly.
 
  This example code is in the public domain.
 */

// the setup routine runs once when you press reset:
void setup() {                
  // initialize the digital pin as an output.
  Serial.begin(57600);
}

// the loop routine runs over and over again forever:
void loop() 
{
  LedReading setting = {0, 0, 0};
      
  setting.green = 128;
  Bean.setLed(setting.red, setting.green, setting.blue);
  
  Bean.sleep(1000);               // wait for a second
  
  setting.green = 0;
  Bean.setLed(setting.red, setting.green, setting.blue);
  
  Bean.sleep(1000);               // wait for a second
}
