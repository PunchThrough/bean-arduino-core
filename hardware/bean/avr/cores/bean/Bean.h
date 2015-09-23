#ifndef BEAN_BEAN_BEAN_H
#define BEAN_BEAN_BEAN_H
#include "BeanSerialTransport.h"
#include "BeanHID.h"

// Accel Events.
#define FLAT_EVENT 0x80
#define ORIENT_EVENT 0x40
#define SINGLE_TAP_EVENT 0x20
#define DOUBLE_TAP_EVENT 0x10
#define ANY_MOTION_EVENT 0x04
#define HIGH_G_EVENT 0x02
#define LOW_G_EVENT 0x01

typedef ACC_READING_T AccelerationReading;
typedef LED_SETTING_T LedReading;
typedef ADV_SWITCH_ENABLED_T BluetoothServices;
typedef ANCS_SOURCE_MSG_T AncsNotification;
typedef NOTI_ATTR_ID_T AncsNotificationAttribute;
typedef OBSERVER_INFO_MESSAGE_T ObseverAdvertisementInfo;

class BeanClass {

 public:
  //************************************************************
  //  Accelerometer
  //************************************************************
  void enableMotionEvent(uint8_t events);
  void disableMotionEvents();
  bool checkMotionEvent(uint8_t events);
  int16_t getAccelerationX(void);
  int16_t getAccelerationY(void);
  int16_t getAccelerationZ(void);
  AccelerationReading getAcceleration(void);
  void accelRegisterWrite(uint8_t reg, uint8_t value);
  int accelRegisterRead(uint8_t reg, uint8_t length, uint8_t *value);
  uint8_t getAccelerationRange(void);
  void setAccelerationRange(uint8_t range);
  void setAccelerometerPowerMode(uint8_t mode);
  uint8_t getAccelerometerPowerMode();

  //************************************************************
  //  LED
  //************************************************************
  void setLed(uint8_t red, uint8_t green, uint8_t blue);
  LedReading getLed(void);
  uint8_t getLedRed(void);
  uint8_t getLedGreen(void);
  uint8_t getLedBlue(void);
  void setLedRed(uint8_t intensity);
  void setLedGreen(uint8_t intensity);
  void setLedBlue(uint8_t intensity);

  //************************************************************
  //  MIDI
  //************************************************************
  void enableMidi(void);
  int midiPacketSend();
  int midiSend(uint8_t *buff, uint8_t numBytes);
  int midiSend(uint8_t status, uint8_t byte1, uint8_t byte2);
  int midiRead(uint8_t *status, uint8_t *byte1, uint8_t *byte2);

  //************************************************************
  // HID
  //************************************************************
  void enableHID(void);
  int HIDPressKey(uint8_t k);
  int HIDReleaseKey(uint8_t k);
  int HIDWriteKey(uint8_t k);
  int HIDWrite(String s);
  void HIDMoveMouse(signed char x, signed char y, signed char wheel = 0);
  void HIDClickMouse(uint8_t b = MOUSE_LEFT);
  void HIDSendConsumerControl(unsigned char command);

  //************************************************************
  //  ANCS
  //************************************************************
  void enableANCS(void);
  int ancsAvailable();
  int readAncs(uint8_t *buffer, size_t max_length);
  int parseAncs(ANCS_SOURCE_MSG_T *buffer, size_t max_length);
  int requestAncsNotiDetails(NOTI_ATTR_ID_T type, size_t len, uint32_t ID);
  int readAncsNotiDetails(uint8_t *buf, size_t max_length);
  void performAncsAction(uint32_t ID, uint8_t actionID);

  //************************************************************
  //  Observer
  //************************************************************
  void startObserver(void);
  void stopObserver(void);
  int getObserverMessage(ObseverAdvertisementInfo *message, unsigned long timeout);

  //************************************************************
  //  Scratch
  //************************************************************
  bool setScratchData(uint8_t bank, const uint8_t *data, uint8_t dataLength);
  bool setScratchNumber(uint8_t bank, uint32_t data);
  ScratchData readScratchData(uint8_t bank);
  long readScratchNumber(uint8_t bank);

  //************************************************************
  //  Sleep
  //************************************************************
  void sleep(uint32_t duration_ms);
  void keepAwake(bool enable);
  void enableWakeOnConnect(bool enable);

  //************************************************************
  //  Interrupts
  //************************************************************
  void attachChangeInterrupt(uint8_t pin, void (*userFunc)(void));
  void detachChangeInterrupt(uint8_t pin);

  //************************************************************
  //  Advertising
  //************************************************************
  void setAdvertisingInterval(uint16_t interval_ms);
  void enableAdvertising(bool enable, uint32_t timer);
  void enableAdvertising(bool enable);
  bool getAdvertisingState(void);
  void enableCustom(void);
  void setCustomAdvertisement(uint8_t *buf, int len);

  //************************************************************
  //  iBeacon
  //************************************************************
  void enableiBeacon(void);
  void setBeaconParameters(uint16_t uuid, uint16_t major_id, uint16_t minor_id);
  void setBeaconEnable(bool beaconEnable);

  //************************************************************
  //  Battery
  //************************************************************
  uint8_t getBatteryLevel(void);
  uint16_t getBatteryVoltage(void);

  //************************************************************
  //  Temperature
  //************************************************************
  int8_t getTemperature(void);

  //************************************************************
  //  Other
  //************************************************************
  void disconnect(void);
  bool getConnectionState(void);
  void setBeanName(const String &s);
  const char *getBeanName(void);
  BluetoothServices getServices(void);
  void setServices(BluetoothServices services);
  void resetServices(void);
  void enableConfigSave(bool enableSave);

  BeanClass() {}

 private:
  bool attemptSleep(uint32_t duration_ms);
  int16_t convertAcceleration(uint8_t high_byte, uint8_t low_byte);
  void accelerometerConfig(uint16_t interrupts, uint8_t power_mode);
  void enableWakeOnAccelerometer(uint8_t sources);
  uint8_t checkAccelInterrupts();

  uint8_t lastStatus;
  long midiTimeStampDiff;
  bool midiPacketBegin;
};

extern BeanClass Bean;

#endif
