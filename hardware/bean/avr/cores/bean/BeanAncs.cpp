#include "BeanAncs.h"
#include "BeanSerialTransport.h"

BeanAncsClass BeanAncs;

void BeanAncsClass::enable(void) {
  ADV_SWITCH_ENABLED_T curServices = Bean.getServices();
  curServices.ancs = 1;
  Bean.setServices(curServices);
}

bool BeanAncsClass::isEnabled(void) {
  ADV_SWITCH_ENABLED_T curServices = Bean.getServices();
  return (curServices.ancs == 1);
}

void BeanAncsClass::disable(void) {
  ADV_SWITCH_ENABLED_T curServices = Bean.getServices();
  curServices.ancs = 0;
  Bean.setServices(curServices);
}

int BeanAncsClass::notificationsAvailable() { return Serial.ancsAvailable(); }


int BeanAncsClass::getNotificationHeaders(ANCS_SOURCE_MSG_T *buffer, size_t max_length) {
  int numMsgs = Serial.ancsAvailable();
  int bytes_written = Serial.readAncs((uint8_t *)buffer, min(max_length, numMsgs) * 8);

  return bytes_written/8;
}

ANCS_SOURCE_MSG_T BeanAncsClass::getNotificationHeader() {
  ANCS_SOURCE_MSG_T msg = {0};
  Serial.readAncs((uint8_t *)&msg, 8);
  return msg;
}

int BeanAncsClass::getNotificationAttributes(NOTI_ATTR_ID_T type, uint32_t ID,
                                                uint16_t len, uint8_t* data,
                                                    uint32_t timeout) {
  uint8_t reqBuf[8];
  reqBuf[0] = 0;  // get notification attributes command ID == 0
  memcpy((void *)&reqBuf[1], &ID, 4);
  reqBuf[5] = type;
  reqBuf[6] = len & 0xFF;
  reqBuf[7] = (len >> 8) & 0xFF;
  return Serial.getAncsNotiDetails(reqBuf, sizeof(reqBuf), data, timeout);
}

void BeanAncsClass::notificationAction(uint32_t ID, uint8_t actionID) {
  uint8_t reqBuf[6];
  reqBuf[0] = 2;  // command ID perform notifcation action
  memcpy((void *)&reqBuf[1], &ID, sizeof(uint32_t));
  reqBuf[5] = actionID;
  Serial.write_message(MSG_ID_ANCS_GET_NOTI, reqBuf, sizeof(reqBuf));
}


