#ifndef BEAN_ANCS_H
#define BEAN_ANCS_H

#include "Arduino.h"
#include "applicationMessageHeaders/AppMessages.h"

/**
 *  Needs docs
 */
typedef ANCS_SOURCE_MSG_T AncsNotification;

/**
 *  Needs docs
 */
typedef NOTI_ATTR_ID_T AncsNotificationAttribute;


class BeanAncsClass {

public:
  /****************************************************************************/
  /** @name ANCS
   *  Use the <a href="https://developer.apple.com/library/ios/documentation/CoreBluetooth/Reference/AppleNotificationCenterServiceSpecification/Introduction/Introduction.html">Apple Notification Center Service</a> to read notifications from your iOS device.
   */
  ///@{

  /**
   *  Needs docs
   */
  void enable(void);

  /**
   *  Needs docs
   */
  bool isEnabled(void);

  /**
   *  Needs docs
   */
  void disable(void);

  /**
   *  Needs docs
   */
  int notificationsAvailable();

  /**
   *  Needs docs
   */
  int getNotificationHeaders(ANCS_SOURCE_MSG_T *buffer, size_t max_length);

  /**
   *  Needs docs
   */
  ANCS_SOURCE_MSG_T getNotificationHeader();

  /**
   *  Needs docs
   */
  int getNotificationAttributes(NOTI_ATTR_ID_T type, uint32_t ID, 
												uint16_t len, uint8_t* data, 
													uint32_t timeout);

  /**
   *  Needs docs
   */
  int getAppAttributes(NOTI_ATTR_ID_T type, uint32_t ID, 
                                                size_t len, uint8_t* data,
                                                    uint32_t timeout);

  /**
   *  Needs docs
   */
  void notificationAction(uint32_t ID, uint8_t actionID);
  ///@}
};

extern BeanAncsClass BeanAncs;

#endif