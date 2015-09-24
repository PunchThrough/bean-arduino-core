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
  /****************************************************************************
  /** @name Accelerometer
   *  Functions related to the Accelerometer
   */
  ///@{

  /**
   *  Needs docs
   */
  void enableMotionEvent(uint8_t events);

  /**
   *  Needs docs
   */
  void disableMotionEvents();

  /**
   *  Needs docs
   */
  bool checkMotionEvent(uint8_t events);

  /**
   *  Needs docs
   */
  int16_t getAccelerationX(void);

  /**
   *  Needs docs
   */
  int16_t getAccelerationY(void);

  /**
   *  Needs docs
   */
  int16_t getAccelerationZ(void);

  /**
   *  Needs docs
   */
  AccelerationReading getAcceleration(void);

  /**
   *  Needs docs
   */
  void accelRegisterWrite(uint8_t reg, uint8_t value);

  /**
   *  Needs docs
   */
  int accelRegisterRead(uint8_t reg, uint8_t length, uint8_t *value);

  /**
   *  Needs docs
   */
  uint8_t getAccelerationRange(void);

  /**
   *  Needs docs
   */
  void setAccelerationRange(uint8_t range);

  /**
   *  Needs docs
   */
  void setAccelerometerPowerMode(uint8_t mode);

  /**
   *  Needs docs
   */
  uint8_t getAccelerometerPowerMode();
  ///@}


  /****************************************************************************
  /** @name LED
   *  Functions related to LED controls
   */
  ///@{

  /**
   *  Needs docs
   */
  void setLed(uint8_t red, uint8_t green, uint8_t blue);

  /**
   *  Needs docs
   */
  LedReading getLed(void);

  /**
   *  Needs docs
   */
  uint8_t getLedRed(void);

  /**
   *  Needs docs
   */
  uint8_t getLedGreen(void);

  /**
   *  Needs docs
   */
  uint8_t getLedBlue(void);

  /**
   *  Needs docs
   */
  void setLedRed(uint8_t intensity);

  /**
   *  Needs docs
   */
  void setLedGreen(uint8_t intensity);

  /**
   *  Needs docs
   */
  void setLedBlue(uint8_t intensity);
  ///@}


  /****************************************************************************
  /** @name MIDI
   *  Functions related to MIDI
   */
  ///@{

  /**
   *  Needs docs
   */
  void enableMidi(void);

  /**
   *  Needs docs
   */
  int midiPacketSend();

  /**
   *  Needs docs
   */
  int midiSend(uint8_t *buff, uint8_t numBytes);

  /**
   *  Needs docs
   */
  int midiSend(uint8_t status, uint8_t byte1, uint8_t byte2);

  /**
   *  Needs docs
   */
  int midiRead(uint8_t *status, uint8_t *byte1, uint8_t *byte2);
  ///@}


  /****************************************************************************
  /** @name HID
   *  Functions related to HID
   */
  ///@{

  /**
   *  Needs docs
   */
  void enableHID(void);

  /**
   *  Needs docs
   */
  int HIDPressKey(uint8_t key);

  /**
   *  Needs docs
   */
  int HIDReleaseKey(uint8_t key);

  /**
   *  Needs docs
   */
  int HIDWriteKey(uint8_t key);

  /**
   *  Needs docs
   */
  int HIDWrite(String charsToType);

  /**
   *  Needs docs
   */
  void HIDMoveMouse(signed char x, signed char y, signed char wheel = 0);

  /**
   *  Needs docs
   */
  void HIDClickMouse(uint8_t b = MOUSE_LEFT);

  /**
   *  Needs docs
   */
  void HIDSendConsumerControl(unsigned char command);
  ///@}


  /****************************************************************************
  /** @name ANCS
   *  Functions related to ANCS
   */
  ///@{

  /**
   *  Needs docs
   */
  void enableANCS(void);

  /**
   *  Needs docs
   */
  int ancsAvailable();

  /**
   *  Needs docs
   */
  int readAncs(uint8_t *buffer, size_t max_length);

  /**
   *  Needs docs
   */
  int parseAncs(ANCS_SOURCE_MSG_T *buffer, size_t max_length);

  /**
   *  Needs docs
   */
  int requestAncsNotiDetails(NOTI_ATTR_ID_T type, size_t len, uint32_t ID);

  /**
   *  Needs docs
   */
  int readAncsNotiDetails(uint8_t *buf, size_t max_length);

  /**
   *  Needs docs
   */
  void performAncsAction(uint32_t ID, uint8_t actionID);
  ///@}


  /****************************************************************************
  /** @name Observer
   *  Functions related to Observer mode
   */
  ///@{

  /**
   *  Needs docs
   */
  void startObserver(void);

  /**
   *  Needs docs
   */
  void stopObserver(void);

  /**
   *  Needs docs
   */
  int getObserverMessage(ObseverAdvertisementInfo *message, unsigned long timeout);
  ///@}


  /****************************************************************************
  /** @name Scratch
   *  Functions related to scratch characteristics
   */
  ///@{

  /**
   *  Needs docs
   */
  bool setScratchData(uint8_t bank, const uint8_t *data, uint8_t dataLength);

  /**
   *  Needs docs
   */
  bool setScratchNumber(uint8_t bank, uint32_t data);

  /**
   *  Needs docs
   */
  ScratchData readScratchData(uint8_t bank);

  /**
   *  Needs docs
   */
  long readScratchNumber(uint8_t bank);
  ///@}


  /****************************************************************************
  /** @name Sleep
   *  Functions related to sleeping and waking
   */
  ///@{

  /**
   *  Needs docs
   */
  void sleep(uint32_t duration_ms);

  /**
   *  Needs docs
   */
  void keepAwake(bool enable);

  /**
   *  Needs docs
   */
  void enableWakeOnConnect(bool enable);
  ///@}


  /****************************************************************************
  /** @name Interrupts
   *  Functions related to interrupts
   */
  ///@{

  /**
   *  Needs docs
   */
  void attachChangeInterrupt(uint8_t pin, void (*userFunc)(void));

  /**
   *  Needs docs
   */
  void detachChangeInterrupt(uint8_t pin);
  ///@}


  /****************************************************************************
  /** @name Advertising
   *  Functions related to advertising
   */
  ///@{

  /**
   *  Needs docs
   */
  void setAdvertisingInterval(uint16_t interval_ms);

  /**
   *  Needs docs
   */
  void enableAdvertising(bool enable, uint32_t timer);

  /**
   *  Needs docs
   */
  void enableAdvertising(bool enable);

  /**
   *  Needs docs
   */
  bool getAdvertisingState(void);

  /**
   *  Needs docs
   */
  void enableCustom(void);

  /**
   *  Needs docs
   */
  void setCustomAdvertisement(uint8_t *buf, int len);
  ///@}


  /****************************************************************************
  /** @name iBeacon
   *  Functions related to iBeacon
   */
  ///@{

  /**
   *  Needs docs
   */
  void enableiBeacon(void);

  /**
   *  Needs docs
   */
  void setBeaconParameters(uint16_t uuid, uint16_t major_id, uint16_t minor_id);

  /**
   *  Needs docs
   */
  void setBeaconEnable(bool beaconEnable);
  ///@}


  /****************************************************************************
  /** @name Battery
   *  Functions related to the battery
   */
  ///@{

  /**
   *  Needs docs
   */
  uint8_t getBatteryLevel(void);

  /**
   *  Needs docs
   */
  uint16_t getBatteryVoltage(void);
  ///@}


  /****************************************************************************
  /** @name Temperature
   *  Functions related to temperature
   */
  ///@{

  /**
   *  Needs docs
   */
  int8_t getTemperature(void);
  ///@}


  /****************************************************************************
  /** @name Connection
   *  Functions related to connection state
   */
  ///@{

  /**
   *  Terminate the connection between Bean and the connected BLE Central
   *  device. If no Central device is connected, this method does nothing.
   */
  void disconnect(void);

  /**
   *  Check if any BLE Central devices are currently connected to Bean.
   *
   *  @return true if a device is connected, false otherwise
   */
  bool getConnectionState(void);
  ///@}


  /****************************************************************************
  /** @name Name
   *  Functions related to naming the Bean
   */
  ///@{

  /**
   *  Needs docs
   */
  void setBeanName(const String &name);

  /**
   *  Needs docs
   */
  const char *getBeanName(void);
  //@}


  /****************************************************************************
  /** @name Other
   *  Functions that don't have a home
   */
  ///@{

  /**
   *  Needs docs
   */
  BluetoothServices getServices(void);

  /**
   *  Needs docs
   */
  void setServices(BluetoothServices services);

  /**
   *  Needs docs
   */
  void resetServices(void);

  /**
   *  Needs docs
   */
  void enableConfigSave(bool enableSave);
  ///@}


  BeanClass() {}

 private:
  /**
   *  Needs docs
   */
  bool attemptSleep(uint32_t duration_ms);

  /**
   *  Needs docs
   */
  int16_t convertAcceleration(uint8_t high_byte, uint8_t low_byte);

  /**
   *  Needs docs
   */
  void accelerometerConfig(uint16_t interrupts, uint8_t power_mode);

  /**
   *  Needs docs
   */
  void enableWakeOnAccelerometer(uint8_t sources);

  /**
   *  Needs docs
   */
  uint8_t checkAccelInterrupts();

  /**
   *  Needs docs
   */

  uint8_t lastStatus;

  /**
   *  Needs docs
   */
  long midiTimeStampDiff;

  /**
   *  Needs docs
   */
  bool midiPacketBegin;
};

/**
 *  Needs docs
 */
extern BeanClass Bean;

#endif
