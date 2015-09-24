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

/**
 *  An acceleration reading from the Bean accelerometer, the BMA250 (<a href="http://ae-bst.resource.bosch.com/media/products/dokumente/bma250/bst-bma250-ds002-05.pdf">datasheet</a>). Also includes the current sensitivity setting.
 *
 *  The accelerometer has 10-bit resolution. Readings are returned raw in the range of -512 to 511. These readings need to be converted to g (<a href="https://en.wikipedia.org/wiki/G-force">g-force</a>).
 *
 *  The accelerometer has configurable sensitivity. When using the accelerometer at the default sensitivity of ±2g, a value of +1 corresponds to +3.91 mg.
 *
 *  The accelerometer sensitivity is ±2g, ±4g, ±8g, or ±16g. These are represented by the values 2, 4, 8, or 16.
 */
typedef ACC_READING_T AccelerationReading;

/**
 *  Intensity values for the color channels of the Bean RGB LED. 0 is off and 255 is on.
 */
typedef LED_SETTING_T LedReading;

/**
 *  Needs docs
 */
typedef ADV_SWITCH_ENABLED_T BluetoothServices;

/**
 *  Needs docs
 */
typedef ANCS_SOURCE_MSG_T AncsNotification;

/**
 *  Needs docs
 */
typedef NOTI_ATTR_ID_T AncsNotificationAttribute;

/**
 *  Needs docs
 */
typedef OBSERVER_INFO_MESSAGE_T ObseverAdvertisementInfo;

class BeanClass {
 public:
  /****************************************************************************/
  /** @name Accelerometer
   *  Read acceleration values and configure the accelerometer's sensitivity.
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
   *  Get the current value of the Bean accelerometer X axis.
   *
   *  @return a 10-bit value corresponding to the current X axis acceleration
   */
  int16_t getAccelerationX(void);

  /**
   *  Get the current value of the Bean accelerometer Y axis.
   *
   *  @return a 10-bit value corresponding to the current Y axis acceleration
   */
  int16_t getAccelerationY(void);

  /**
   *  Get the current value of the Bean accelerometer Z axis.
   *
   *  @return a 10-bit value corresponding to the current Z axis acceleration
   */
  int16_t getAccelerationZ(void);

  /**
   *  Get the current value of all axes and the current sensitivity setting from the Bean accelerometer.
   *
   *  @return an AccelerationReading object containing current acceleration and sensitivity
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
   *  Get the current sensitivity setting of the Bean accelerometer.
   *
   *  @return 2, 4, 8, or 16, corresponding to ±2g, ±4g, ±8g, or ±16g
   */
  uint8_t getAccelerationRange(void);

  /**
   *  Configure the sensitivity of the Bean accelerometer.
   *
   *  @param range 2, 4, 8, or 16, corresponding to ±2g, ±4g, ±8g, or ±16g
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


  /****************************************************************************/
  /** @name LED
   *  Set the RGB LED color and check what it's currently showing.
   */
  ///@{

  /**
   *  Set intensity values for the color channels of the Bean RGB LED. 0 is off and 255 is on.
   */
  void setLed(uint8_t red, uint8_t green, uint8_t blue);

  /**
   *  Get current intensity values for the color channels of the Bean RGB LED.
   */
  LedReading getLed(void);

  /**
   *  Get intensity of the red channel of the Bean RGB LED. 0 is off and 255 is on.
   */
  uint8_t getLedRed(void);

  /**
   *  Get intensity of the green channel of the Bean RGB LED. 0 is off and 255 is on.
   */
  uint8_t getLedGreen(void);

  /**
   *  Get intensity of the blue channel of the Bean RGB LED. 0 is off and 255 is on.
   */
  uint8_t getLedBlue(void);

  /**
   *  Set intensity of the red channel of the Bean RGB LED. 0 is off and 255 is on.
   */
  void setLedRed(uint8_t intensity);

  /**
   *  Set intensity of the green channel of the Bean RGB LED. 0 is off and 255 is on.
   */
  void setLedGreen(uint8_t intensity);

  /**
   *  Set intensity of the blue channel of the Bean RGB LED. 0 is off and 255 is on.
   */
  void setLedBlue(uint8_t intensity);
  ///@}


  /****************************************************************************/
  /** @name MIDI
   *  Read and write MIDI packets via Bluetooth Low Energy.
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


  /****************************************************************************/
  /** @name HID
   *  Use your Bean as a Human Interface Device to emulate a keyboard or mouse.
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


  /****************************************************************************/
  /** @name ANCS
   *  Use the <a href="https://developer.apple.com/library/ios/documentation/CoreBluetooth/Reference/AppleNotificationCenterServiceSpecification/Introduction/Introduction.html">Apple Notification Center Service</a> to read notifications from your iOS device.
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


  /****************************************************************************/
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


  /****************************************************************************/
  /** @name Scratch
   *  Read and write arbitrary data using fixed BLE characteristics available on Bean.
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


  /****************************************************************************/
  /** @name Sleep
   *  Bean power management functions to help save battery life.
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


  /****************************************************************************/
  /** @name Interrupts
   *  Call functions when events occur using pin change interrupts.
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


  /****************************************************************************/
  /** @name Advertising
   *  Set and verify the Bean BLE advertising configuration.
   */
  ///@{

  /**
   *  Set the advertising name of the Bean. BLE advertising names are truncated at 20 bytes.
   *
   *  @param s The name to be advertised
   */
  void setBeanName(const String &s);

  /**
   *  Read the currently-advertised name of the Bean.
   *
   *  @return The Bean name as a char array, null-terminated
   */
  const char *getBeanName(void);

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


  /****************************************************************************/
  /** @name iBeacon
   *  Let your Bean act as an <a href="https://developer.apple.com/ibeacon/">iBeacon</a>, a way to convey real-world location to iOS devices.
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


  /****************************************************************************/
  /** @name Battery
   *  Read the Bean's battery level.
   */
  ///@{

  /**
   *  Get the current battery level, in percent.
   *
   *  @return a value in the range 0 to 100: 0 = 1.95 V, 100 = 3.53 V
   */
  uint8_t getBatteryLevel(void);

  /**
   *  Get the current battery voltage, in volts.
   *
   *  Accuracy is ±0.01 V.
   *
   *  @return a value in the range 195 to 353: 195 = 1.95 V, 353 = 3.53 V
   */
  uint16_t getBatteryVoltage(void);
  ///@}


  /****************************************************************************/
  /** @name Temperature
   *  Read the ambient temperature of the Bean.
   */
  ///@{

  /**
   *  Get the current temperature of the Bean, in degrees Celsius. The Bean uses the BMA250 (<a href="http://ae-bst.resource.bosch.com/media/products/dokumente/bma250/bst-bma250-ds002-05.pdf">datasheet</a>) for temperature readings.
   *
   *  @return temperature, between -40 and 88 degrees Celsius
   */
  int8_t getTemperature(void);
  ///@}


  /****************************************************************************/
  /** @name Connection
   *  Work with active BLE Central devices that connect to Bean.
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


  /***************************************************************************/
  /** @name Bluetooth Services
   *  Needs description
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
  ///@}


  /***************************************************************************/
  /** @name Other
   *  Functions that don't belong in any of the other categories.
   */
  ///@{

  /**
   *  Allows temporary storage of BLE configuration settings, as opposed to permanent storage in non-volatile memory (NVRAM).
   *
   *  Bean can only write to NVRAM a maximum of 20,000 times. If your sketch changes its advertising configuration frequently, it will quickly use up all of your Bean NVRAM's writes. To prevent this, disable saving to NVRAM first.
   *
   *  Configurations written while NVRAM saving is disabled will persist until the Bean is power cycled.
   *
   *  When the Bean is power cycled, saving to NVRAM is re-enabled.
   *
   *  @param enableSave true to disable saving to NVRAM, false to enable
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
