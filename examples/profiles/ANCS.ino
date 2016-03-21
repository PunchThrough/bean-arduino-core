/*
 * This allows the Bean to act as an ANCS device.  The Bean must pair using the pin 000000.
 * ANCS is the Apple Notification Center Service that allows a connected bluetooth device to receive information about nofications
 * The Bean can view details about these notifications such as an emails text or a incoming phone call number
 * Download this sketch, reset the bean, then connect to an iOS device and use something like the Bean Console for iOS to view the outputs
 *
 * For details on what different elements mean see Apple's ANCS documentation found here: https://developer.apple.com/library/ios/documentation/CoreBluetooth/Reference/AppleNotificationCenterServiceSpecification/Appendix/Appendix.html#//apple_ref/doc/uid/TP40013460-CH3-SW2
 */



int latestID = 0;
AncsNotification notifications[8];

void setup() {
     Serial.begin();
     BeanAncs.enable();
}
 
 
void loop() {
   int msgAvail = BeanAncs.notificationsAvailable();
   if(msgAvail)
   {
   	 //blink red every time we have a new message
     Bean.setLedGreen(0);
     Bean.setLedRed(130);
     
     //fills the notifications array with message header info
     BeanAncs.getNotificationHeaders(notifications, 8);

     for(int i = 0; i < msgAvail; i ++)
     {

       Serial.print("Cat:");
       Serial.print(notifications[i].catID);
       Serial.print("\n");
       
       Serial.print("flg:");
       Serial.print(notifications[i].flags);
       Serial.print("\n");
       
       Serial.print("evt:");
       Serial.print(notifications[i].eventID);
       Serial.print("\n");
       
       Serial.print("cnt:");
       Serial.print(notifications[i].catCount);
       Serial.print("\n");
       
       Serial.print("id:");
       Serial.print(notifications[i].notiUID);
       Serial.println();
       char data[64] = {0};
       //get some information about each message
       int len = BeanAncs.getNotificationAttributes(NOTI_ATTR_ID_MESSAGE, notifications[i].notiUID, 39, (uint8_t *)data, 5000);
       
       Serial.print("l: ");
       Serial.print(len);
       Serial.print(" ");
       for(int j = 0; j < len; j++)
       {
         Serial.print(data[j]);
       }
       Serial.println();
       delay(5000);
     }
     Bean.setLedRed(0);
   }
   
   //send a number through serial (with newline at end) to perform a negative action
   if(Serial.available() > 0)
   {
   	 byte idin[8] = {0};
   	 int index = 0;
     byte read = Serial.read();
     while(read != '\n' && index < 7)
     {
     	idin[index] = read;
     	index ++;
     	read = Serial.read();
     }

     int id = atoi(idin);
     Serial.print("Performing negative action on ID: ");
     Serial.println(id);
     BeanAncs.notificationAction(id, 0x01);

   } 
}