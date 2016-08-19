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
   *  This allows a connected Bluetooth device to receive information about
   *  iOS nofications. Bean can view details about these notifications, e.g.
   *  push notifications, SMS previews, or an incoming phone call's number.
   *
   *  To use ANCS, upload an ANCS sketch to Bean, power cycle Bean, and pair
   *  with Bean from your iOS device using pairing code 000000.
   *
   *  When you disconnect from an Bean in ANCS mode, you'll need to manually
   *  forget it from the iOS Bluetooth Settings menu.
   */
  ///@{

  /**
   *  Enables ANCS functionality. After uploading an ANCS sketch, make sure to
   *  power cycle Bean.
   */
  void enable(void);

  /**
   *  @return true if ANCS is currently enabled
   */
  bool isEnabled(void);

  /**
   *  Disables ANCS functionality
   */
  void disable(void);

  /**
   *  @return number of notifications available that can be read using getNotificationHeaders
   */
  int notificationsAvailable();

  /**
   *  Gets all available notification headers.  The ANCS buffer can only hold 8 messages at a time before it begins to overwrite old messages.
   *  @param buffer takes a user defined buffer of type ANCS_SOURCE_MSG_T.  When this function returns the buffer will be populated with messages
   *  @param max_length takes the length of the maximum buffer size.  If the user only has a buffer of one then max_length should also be 1.
   *  @return number of messages actually read into the buffer.
   */
  int getNotificationHeaders(ANCS_SOURCE_MSG_T *buffer, size_t max_length);

  /**
   *  Gets a single notification Header.  If there are no messages available all values will be set to 0.  It is up to the user to check using notificationsAvailable() if there are messages to read.
   */
  ANCS_SOURCE_MSG_T getNotificationHeader();

  /**
   *  This function can be used to request details about a particular notification.  It can only be used to access one type of notification at a time and will block the thread until it receives a message.
   *  @param type is the type of data the user wishes to receive of type NOTI_ATTR_ID_T
   *  @param ID is the UUID of the notification as contained in ANCS_SOURCE_MSG_T.
   *  @param len is the max number of bytes to receive into data.  The maximum possible is 65535 bytes.
   *  @param data is the user defined buffer to fill incoming data with.
   *  @param timeout is the length of time in milliseconds before the function will return if it does not receive data or only receives partial data
   *  @param return the number of bytes actually read to data
   *
   *  # Examples
   *
   *  This example sketch reads ANCS messages and prints them to Virtual Serial.
   *  Pair with your iOS device, then use
   *  <a href="https://itunes.apple.com/us/app/bean-console/id982751969?mt=8">Bean Console for iOS</a>
   *  to view data live from your iOS device.
   *
   *  @include profiles/ANCS.ino
   */
  int getNotificationAttributes(NOTI_ATTR_ID_T type, uint32_t ID,
                        uint16_t len, uint8_t* data,
                          uint32_t timeout);

  /**
   *  Certain notifications allow different actions.  For instance many push notifications can be cleared and calls can be answered.
   *  @param ID the UUID of the notification to perform the action on.
   *  @param actionID the ID of the action to perform.  There are only two: ActionIDPositive(0x00) and ActionIDNegative(0x01).  The result of the action is dependent on the notification.
   */
  void notificationAction(uint32_t ID, uint8_t actionID);
  ///@}
};

extern BeanAncsClass BeanAncs;

#endif
