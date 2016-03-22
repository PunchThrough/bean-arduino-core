int latestID = 0;
AncsNotification notifications[8];

void setup() {
  // Serial port is initialized automatically; we don't have to do anything
  BeanAncs.enable();
}

void loop() {
  int msgAvail = BeanAncs.notificationsAvailable();
  if (msgAvail) {
    // blink red every time we have a new message
    Bean.setLedGreen(0);
    Bean.setLedRed(130);

    // fills the notifications array with message header info
    BeanAncs.getNotificationHeaders(notifications, 8);

    for (int i = 0; i < msgAvail; i++) {
      Serial.print("cat:");
      Serial.println(notifications[i].catID);
      Serial.print("flg:");
      Serial.println(notifications[i].flags);
      Serial.print("evt:");
      Serial.println(notifications[i].eventID);
      Serial.print("cnt:");
      Serial.println(notifications[i].catCount);
      Serial.print("id: ");
      Serial.println(notifications[i].notiUID);

      // get some information about each message
      char data[64] = {0};
      int len = BeanAncs.getNotificationAttributes(
        NOTI_ATTR_ID_MESSAGE,
        notifications[i].notiUID,
        39,
        (uint8_t *)data,
        5000);

      Serial.print("l: ");
      Serial.print(len);
      Serial.print(" ");
      for (int j = 0; j < len; j++) {
        Serial.print(data[j]);
      }
      Serial.println();
      delay(5000);
    }
    Bean.setLedRed(0);
  }

  // send a number through serial (with newline at end) to perform a negative
  // action on a message with the given ID
  if (Serial.available() > 0) {
    char idin[8] = {0};
    int index = 0;
    byte read = Serial.read();
    while (read != '\n' && index < 7) {
      idin[index] = read;
      index++;
      read = Serial.read();
    }

    int id = atoi(idin);
    Serial.print("Performing negative action on ID: ");
    Serial.println(id);
    BeanAncs.notificationAction(id, 0x01);
  }
}
