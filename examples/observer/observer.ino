void setup() {
  Serial.begin();
  
}

void loop() {  
  uint32_t start = millis();

    ObseverAdvertisementInfo info;
    int res = Bean.getObserverMessage(&info, 2000);
    if(res == 1)
    {      
      Serial.print(info.eventType);
      Serial.print(',');
      Serial.print(info.addrType);
      Serial.print(',');
      Serial.print("addr: ");
      for(int i = 0; i < 6; i ++)
      {
        Serial.print(info.addr[i]);
        Serial.print(',');
      }
      Serial.print("  advData: ");
      for(int i = 0; i < info.dataLen; i ++)
      {
        Serial.print(info.advData[i], HEX);
        Serial.print(',');
      }
      Serial.println();
      
      
      if(info.advData[0] == 0x02 &&
         info.advData[1] == 0x01 &&
         info.advData[2] == 0x06 &&
         info.advData[3] == 0x03 && 
         info.advData[4] == 0xff)
      {
         if(info.advData[6] == 0)
         {
           Bean.setLedGreen(60);
         }
         else
         {
           Bean.setLedGreen(0);
         }
      }
    }
}