/*
  LightBlue Demo Sketch
 */

static int d0 = 0;
static int d1 = 1;
static int d2 = 2;
static int d3 = 3;
static int d4 = 4;
static int d5 = 5;


void setup() 
{
  // initialize serial communication at 57600 bits per second:
  Serial.begin(57600);

  Serial.setTimeout(25);
  
  // Digital pins
  pinMode(d0, INPUT_PULLUP);  
  pinMode(d1, INPUT_PULLUP);  
  pinMode(d2, INPUT_PULLUP);  
  pinMode(d3, INPUT_PULLUP); 
  pinMode(d4, INPUT_PULLUP);
  pinMode(d5, INPUT_PULLUP);
}

// the loop routine runs over and over again forever:
void loop() 
{
  char buffer[64];
  size_t readLength = 64;
  uint8_t length = 0;  
         
  length = Serial.readBytes(buffer, readLength);    
  
// Return all the serial pins
  if ( 0x02 == buffer[0] && 1 == length)
  {  
      pinMode(d0, INPUT_PULLUP);  //PD6
      pinMode(d1, INPUT_PULLUP);  //PB1
      pinMode(d2, INPUT_PULLUP);  //PB2
      pinMode(d3, INPUT_PULLUP); 
      pinMode(d4, INPUT_PULLUP);
      pinMode(d5, INPUT_PULLUP);
      
      int digital0 = digitalRead(d0);
      int digital1 = digitalRead(d1);
      int digital2 = digitalRead(d2);
      int digital3 = digitalRead(d3);
      int digital4 = digitalRead(d4);
      int digital5 = digitalRead(d5);
      int analog1 = analogRead(A0);
      int analog2 = analogRead(A1);
      
      uint8_t digitalAll = digital0 | ( digital1 << 1 ) | ( digital2 << 2 ) | ( digital3 << 3);
      digitalAll |= ( digital4 << 4 ) | ( digital5 << 5 );
      
      buffer[0] = 0x82;
      buffer[1] = digitalAll;
      buffer[2] = analog1 & 0xFF;
      buffer[3] = analog1 >> 8;
      buffer[4] = analog2 & 0xFF;
      buffer[5] = analog2 >> 8;
    
      Serial.write((uint8_t*)buffer, 6); 
    }

    Bean.sleep(1000000);  
}
