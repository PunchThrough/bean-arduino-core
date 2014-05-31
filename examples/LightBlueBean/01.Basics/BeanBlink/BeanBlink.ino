/*
  The simplest Blink sketch, with red, green, blue.
  
  */

// the setup routine runs once when you press reset:
void setup() {
  // initialize serial communication at 57600 bits per second:
  Serial.begin(57600); 
}

// the loop routine runs over and over again forever:
void loop() {
  Bean.setLed(0,0,255);  //blue
  Bean.sleep(250);
  Bean.setLed(0,255,0);  //green
  Bean.sleep(250);
  Bean.setLed(255,0,0);  //red
  Bean.sleep(250);
  Bean.setLed(0,0,0);    //off
  Bean.sleep(3000);
}

