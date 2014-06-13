void setup()
{
 Serial.begin(57600);

 Serial.setTimeout(25);   
}

void loop()
{
  uint8_t level = 0;
  
  level = Bean.getBatteryLevel();
  
  Serial.print("Battery level%: ");
  Serial.println(level, DEC);
  
  Bean.sleep(1000);
}
