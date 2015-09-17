#ifndef BEAN_BEAN_BEAN_H
#define BEAN_BEAN_BEAN_H
#include "BeanSerialTransport.h"
#include "BeanHID.h"

typedef ACC_READING_T AccelerationReading;
typedef LED_SETTING_T LedReading;

typedef ADV_SWITCH_ENABLED_T BluetoothServices;
typedef ANCS_SOURCE_MSG_T AncsNotification;
typedef NOTI_ATTR_ID_T AncsNotificationAttribute;
typedef OBSERVER_INFO_MESSAGE_T ObseverAdvertisementInfo;

/**
 * TODO: Documentation needed
*/
class BeanClass {

 public:

  //*********************************************************************
  // Accelerometer functions
  //*********************************************************************

  /**
   * TODO: Documentation needed
   */
  int16_t getAccelerationX(void);

  /**
   * TODO: Documentation needed
   */
  int16_t getAccelerationY(void);

  /**
   * TODO: Documentation needed
   */
  int16_t getAccelerationZ(void);

  /**
   * TODO: Documentation needed
   */
  AccelerationReading getAcceleration(void);

  /**
   * TODO: Documentation needed
   */
  uint8_t getAccelerationRange(void);

  /**
   * TODO: Documentation needed
   */
  void setAccelerationRange(uint8_t range);

  /**
   * TODO: Documentation needed
   */
  void setAccelerometerPowerMode(uint8_t mode);

  /**
   * TODO: Documentation needed
   */
  uint8_t getAccelerometerPowerMode();

  /**
   * TODO: Documentation needed
   */
  void accelerometerConfig(uint16_t interrupts, uint8_t power_mode);

  /**
   * TODO: Documentation needed
   */
  void enableWakeOnAccelerometer(uint8_t sources);

  /**
   * TODO: Documentation needed
   */
  void enableAccelSingleTapInt();

  /**
   * TODO: Documentation needed
   */
  void enableAccelDoubleTapInt();

  /**
   * TODO: Documentation needed
   */
  void enableLowGravityInt();

  /**
   * TODO: Documentation needed
   */
  void enableAnyMotionInts();

  /**
   * TODO: Documentation needed
   */
  void disableAccelInterrupts();

  /**
   * TODO: Documentation needed
   */
  uint8_t checkAccelInterrupts();

  //*********************************************************************
  // Battery functions
  //*********************************************************************

  /**
   * TODO: Documentation needed
   */
  uint8_t getBatteryLevel(void);

  /**
   * TODO: Documentation needed
   */
  uint16_t getBatteryVoltage(void);

  //*********************************************************************
  // LED functions
  //*********************************************************************

  /**
   * TODO: Documentation needed
   */
  void setLed(uint8_t red, uint8_t green, uint8_t blue);

  /**
   * TODO: Documentation needed
   */
  LedReading getLed(void);

  /**
   * TODO: Documentation needed
   */
  uint8_t getLedRed(void);

  /**
   * TODO: Documentation needed
   */
  uint8_t getLedGreen(void);

  /**
   * TODO: Documentation needed
   */
  uint8_t getLedBlue(void);

  /**
   * TODO: Documentation needed
   */
  void setLedRed(uint8_t intensity);

  /**
   * TODO: Documentation needed
   */
  void setLedGreen(uint8_t intensity);

  /**
   * TODO: Documentation needed
   */
  void setLedBlue(uint8_t intensity);

  //*********************************************************************
  // BLE services functions
  //*********************************************************************

  /**
   * TODO: Documentation needed
   */
  BluetoothServices getServices(void);

  /**
   * TODO: Documentation needed
   */
  void setServices(BluetoothServices services);

  /**
   * TODO: Documentation needed
   */
  void resetServices(void);

  //*********************************************************************
  // MIDI functions
  //*********************************************************************

  /**
   * TODO: Documentation needed
   */
  void enableMidi(void);

  /**
   * TODO: Documentation needed
   */
  int midiPacketSend();

  /**
   * TODO: Documentation needed
   */
  int midiSend(uint8_t *buff, uint8_t numBytes);

  /**
   * TODO: Documentation needed
   */
  int midiSend(uint8_t status, uint8_t byte1, uint8_t byte2);

  /**
   * TODO: Documentation needed
   */
  int midiRead(uint8_t &status, uint8_t &byte1, uint8_t &byte2);

  //*********************************************************************
  // HID functions
  //*********************************************************************

  /**
   * TODO: Documentation needed
   */
  void enableHID(void);

  /**
   * TODO: Documentation needed
   */
  int HIDPressKey(uint8_t k);

  /**
   * TODO: Documentation needed
   */
  int HIDReleaseKey(uint8_t k);

  /**
   * TODO: Documentation needed
   */
  int HIDWriteKey(uint8_t k);

  /**
   * TODO: Documentation needed
   */
  int HIDWrite(String s);

  /**
   * TODO: Documentation needed
   */
  void HIDMoveMouse(signed char x, signed char y, signed char wheel = 0);

  /**
   * TODO: Documentation needed
   */
  void HIDClickMouse(uint8_t b = MOUSE_LEFT);

  /**
   * TODO: Documentation needed
   */
  void HIDSendConsumerControl(unsigned char command);

  //*********************************************************************
  // ANCS functions
  //*********************************************************************

  /**
   * TODO: Documentation needed
   */
  void enableANCS(void);

  /**
   * TODO: Documentation needed
   */
  int ancsAvailable();

  /**
   * TODO: Documentation needed
   */
  int readAncs(uint8_t *buffer, size_t max_length);

  /**
   * TODO: Documentation needed
   */
  int parseAncs(ANCS_SOURCE_MSG_T *buffer, size_t max_length);

  /**
   * TODO: Documentation needed
   */
  int requestAncsNotiDetails(NOTI_ATTR_ID_T type, size_t len, uint32_t ID);

  /**
   * TODO: Documentation needed
   */
  int readAncsNotiDetails(uint8_t *buf, size_t max_length);

  /**
   * TODO: Documentation needed
   */
  void performAncsAction(uint32_t ID, uint8_t actionID);

  //*********************************************************************
  // Observer functions
  //*********************************************************************

  /**
   * TODO: Documentation needed
   */
  void startObserver(void);

  /**
   * TODO: Documentation needed
   */
  void stopObserver(void);

  /**
   * TODO: Documentation needed
   */
  int getObserverMessage(ObseverAdvertisementInfo *message,
                         unsigned long timeout);

  //*********************************************************************
  // Scratch functions
  //*********************************************************************

  /**
   * TODO: Documentation needed
   */
  bool setScratchData(uint8_t bank, const uint8_t *data, uint8_t dataLength);

  /**
   * TODO: Documentation needed
   */
  bool setScratchNumber(uint8_t bank, uint32_t data);

  /**
   * TODO: Documentation needed
   */
  ScratchData readScratchData(uint8_t bank);

  /**
   * TODO: Documentation needed
   */
  long readScratchNumber(uint8_t bank);

  //*********************************************************************
  // Sleep functions
  //*********************************************************************

  /**
   * TODO: Documentation needed
   */
  void sleep(uint32_t duration_ms);

  /**
   * TODO: Documentation needed
   */
  void keepAwake(bool enable);

  //*********************************************************************
  // Interrupt functions
  //*********************************************************************

  /**
   * TODO: Documentation needed
   */
  void attachChangeInterrupt(uint8_t pin, void (*userFunc)(void));

  /**
   * TODO: Documentation needed
   */
  void detachChangeInterrupt(uint8_t pin);

  //*********************************************************************
  // Advertising functions
  //*********************************************************************

  /**
   * TODO: Documentation needed
   */
  void setCustomAdvertisement(uint8_t *buf, int len);

  /**
   * TODO: Documentation needed
   */
  void setAdvertisingInterval(uint16_t interval_ms);

  /**
   * TODO: Documentation needed
   */
  void enableAdvertising(bool enable, uint32_t timer);

  /**
   * TODO: Documentation needed
   */
  void enableAdvertising(bool enable);

  /**
   * TODO: Documentation needed
   */
  bool getAdvertisingState(void);

  //*********************************************************************
  // iBeacon functions
  //*********************************************************************

  /**
   * TODO: Documentation needed
   */
  void enableiBeacon(void);

  /**
   * TODO: Documentation needed
   */
  void setBeaconParameters(uint16_t uuid, uint16_t major_id, uint16_t minor_id);

  /**
   * TODO: Documentation needed
   */
  void setBeaconEnable(bool beaconEnable);

  //*********************************************************************
  // Other
  //*********************************************************************

  /**
   * TODO: Documentation needed
   */
  int8_t getTemperature(void);

  /**
   * TODO: Documentation needed
   */
  void enableConfigSave(bool enableSave);

  /**
   * TODO: Documentation needed
   */
  void enableCustom(void);

  /**
   * TODO: Documentation needed
   */
  void setBeanName(const String &s);

  /**
   * TODO: Documentation needed
   */
  const char *getBeanName(void);

  /**
   * TODO: Documentation needed
   */
  void enableWakeOnConnect(bool enable);

  /**
   * TODO: Documentation needed
   */
  void disconnect(void);

  /**
   * TODO: Documentation needed
   */
  bool getConnectionState(void);

  BeanClass() {}

 private:
  bool attemptSleep(uint32_t duration_ms);
  int16_t convertAcceleration(uint8_t high_byte, uint8_t low_byte);

  uint8_t lastStatus;
  long midiTimeStampDiff;
  bool midiPacketBegin;
};

extern BeanClass Bean;

#endif
