/*
  Led from Acceleromter.
  
  Read the accelerometer every half second. 
  Set the LED to something based on what we read. 
  */

// the setup routine runs once when you press reset:
void setup() {
  // initialize serial communication at 57600 bits per second:
  Serial.begin(57600);
}

// the loop routine runs over and over again forever:
void loop() {
    LED_SETTING_T setting = {0, 0, 0};
    ACC_READING_T accel = {0, 0, 0};
    
    if(Serial.accelRead(&accel) != 0){
     //Serial.print("Error reading\n"); 
    }
    else {
       // 0 -> 2400 / 10 == range of 0 -> 240. Pretty good for our LED
       setting.red = (accel.xAxis + 1200) / 10;
       setting.green = (accel.yAxis + 1200) / 10;
       setting.blue = (accel.zAxis + 1200) / 10;
       Serial.ledSet(setting);     
    }
        
    delay(500);
}



