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
   *
   *  # Examples
   *
   *  This example sets the LED to white (all channels on, full intensity):
   *  @include led/setLed.ino
   */
  void setLed(uint8_t red, uint8_t green, uint8_t blue);

  /**
   *  Get current intensity values for the color channels of the Bean RGB LED.
   * 
   *  @return `LedReading` struct that contains an integer representation of each color.
   *
   *  # Examples
   *  
   *  This example shows the usage of the getLed() function and how to interperet the return value.
   *  @include led/getLed.ino
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
  void midi_enable(void);

  /**
   *  Needs docs
   */
  int midi_sendMessage(uint8_t *buff, uint8_t numBytes);

  /**
   *  Needs docs
   */
  int midi_sendMessage(uint8_t status, uint8_t byte1, uint8_t byte2);

  /**
   *  Needs docs
   */
  int midi_readMessage(uint8_t *status, uint8_t *byte1, uint8_t *byte2);

  /**
   *  Needs docs
   */
  int midi_sendMessages();

  /**
   *  Needs docs
   */
  int midi_loadMessage(uint8_t *buff, uint8_t numBytes);

  /**
   *  Needs docs
   */
  int midi_loadMessage(uint8_t status, uint8_t byte1, uint8_t byte2);

  ///@}


  /****************************************************************************/
  /** @name HID
   *  Use your Bean as a Human Interface Device to emulate a keyboard or mouse.
   */
  ///@{

  /**
   *  Needs docs
   */
  void hid_enable(void);

  /**
   *  Needs docs
   */
  int hid_holdKey(uint8_t key);

  /**
   *  Needs docs
   */
  int hid_releaseKey(uint8_t key);

  /**
   *  Needs docs
   */
  int hid_sendKey(uint8_t key);

  /**
   *  Needs docs
   */
  int hid_sendKeys(String charsToType);

  /**
   *  Needs docs
   */
  void hid_moveMouse(signed char delta_x, signed char delta_y, signed char delta_wheel = 0);

  /**
   *  Needs docs
   */
  void hid_releaseMouse(mouseButtons button);

  /**
   *  Needs docs
   */
  void hid_holdMouse(mouseButtons button);

  /**
   *  Needs docs
   */
  void hid_sendMouse(mouseButtons button = MOUSE_LEFT);

  /**
   *  Needs docs
   */
  void hid_sendMediaControl(mediaControl command);

  /**
   * Needs docs
   */
  void hid_holdMediaControl(mediaControl command);

  /**
   * Needs docs
   */
  void hid_ReleaseMediaControl(mediaControl command);
  ///@}


  /****************************************************************************/
  /** @name ANCS
   *  Use the <a href="https://developer.apple.com/library/ios/documentation/CoreBluetooth/Reference/AppleNotificationCenterServiceSpecification/Introduction/Introduction.html">Apple Notification Center Service</a> to read notifications from your iOS device.
   */
  ///@{

  /**
   *  Needs docs
   */
  void ancs_enable(void);

  /**
   *  Needs docs
   */
  int ancs_available();

  /**
   *  Needs docs
   */
  int ancs_read(uint8_t *buffer, size_t max_length);

  /**
   *  Needs docs
   */
  int ancs_parse(ANCS_SOURCE_MSG_T *buffer, size_t max_length);

  /**
   *  Needs docs
   */
  int ancs_requestNotiDetails(NOTI_ATTR_ID_T type, size_t len, uint32_t ID);

  /**
   *  Needs docs
   */
  int ancs_readNotiDetails(uint8_t *buf, size_t max_length);

  /**
   *  Needs docs
   */
  void ancs_performAction(uint32_t ID, uint8_t actionID);
  ///@}


  /****************************************************************************/
  /** @name Observer
   *  Functions related to Observer mode
   */
  ///@{

  /**
   *  Needs docs
   */
  void observer_start(void);

  /**
   *  Needs docs
   */
  void observer_stop(void);

  /**
   *  Needs docs
   */
  int observer_getMessage(ObseverAdvertisementInfo *message, unsigned long timeout);
  ///@}


  /****************************************************************************/
  /** @name Scratch
   *  Read and write arbitrary data using pre-defined BLE characteristics.
   *
   *  Scratch characteristics are Bluetooth Low Energy characteristics that Bean provides for arbitrary use by developers. Each characteristic can hold up to 20 bytes due to BLE restrictions.
   *
   *  Scratch characteristics will trigger Notify events on BLE Central clients when they are changed by Bean's Arduino sketch. Bean sketches must poll to find out when a client changes scratch characteristic data.
   *
   *  Bean and Bean+ have five scratch characteristics. All scratch chars are contained in a single BLE service.
   *
   *  * **Scratch Service UUID:** `a495ff20-c5b1-4b44-b512-1370f02d74de`
   *  * **Scratch Characteristic UUIDs:**
   *    1. `a495ff21-c5b1-4b44-b512-1370f02d74de`
   *    2. `a495ff22-c5b1-4b44-b512-1370f02d74de`
   *    3. `a495ff23-c5b1-4b44-b512-1370f02d74de`
   *    4. `a495ff24-c5b1-4b44-b512-1370f02d74de`
   *    5. `a495ff25-c5b1-4b44-b512-1370f02d74de`
   *
   *  In the below methods, behavior is undefined when the `bank` parameter is not `1`, `2`, `3`, `4`, or `5`.
   *
   */
  ///@{

  /**
   *  Set the data in a scratch characteristic. Triggers a BLE Notify event for connected clients.
   *
   *  @param bank         The index of the destination scratch char: `1`, `2`, `3`, `4`, or `5`
   *  @param data         An array of `byte`s or `uint8_t`s to be copied into the scratch char
   *  @param dataLength   The number of bytes to copy from `data`
   *
   *  @return             Success: false if `dataLength` is greater than 20, true otherwise
   *
   *  # Examples
   *
   *  This example reads two of Bean's analog pins and writes the values to two scratch characteristics:
   *  @include scratchChars/setScratchData.ino
   */
  bool setScratchData(uint8_t bank, const uint8_t *data, uint8_t dataLength);

  /**
   *  Write a 32-bit (four-byte) value into a scratch characteristic. Triggers a BLE Notify event for connected clients.
   *
   *  @param bank         The index of the destination scratch char: `1`, `2`, `3`, `4`, or `5`
   *  @param data         The 32-bit value to be written into the scratch char
   *
   *  @return             Success: always returns true, since all 32-bit numbers are under 20 bytes in length
   *
   *  # Examples
   *
   *  This example writes a value to and reads a value from a scratch characteristic:
   *  @include scratchChars/setScratchNumber.ino
   */
  bool setScratchNumber(uint8_t bank, uint32_t data);

  /**
   *  Read the data from a scratch characteristic.
   *
   *  @param bank         The index of the source scratch char: `1`, `2`, `3`, `4`, or `5`
   *
   *  @return             The contents of the scratch characteristic
   *
   *  # Examples
   *
   *  This example polls a scratch characteristic and blinks Bean's LED if the value changes:
   *  @include scratchChars/readScratchData.ino
   */
  ScratchData readScratchData(uint8_t bank);

  /**
   *  Read a 32-bit (four-byte) value from a scratch characteristic.
   *
   *  @param bank         The index of the source scratch char: `1`, `2`, `3`, `4`, or `5`
   *
   *  # Examples
   *
   *  This example writes a value to and reads a value from a scratch characteristic:
   *  @include scratchChars/setScratchNumber.ino
   */
  long readScratchNumber(uint8_t bank);
  ///@}


  /****************************************************************************/
  /** @name Sleep
   *  Bean power management functions to help save battery life.
   */
  ///@{

  /**
   *  Sleep for a period of time.
   *
   *  This method puts the ATmega chip to sleep until it is woken by the LBM313 at the requested time.
   *
   *  Bean can be woken from sleep by the following:
   *
   *  * The requested sleep time elapses
   *  * A serial message is received from a connected client
   *  * A client connects to Bean while wake on connect is enabled
   *  * A pin change interrupt occurs
   *
   *  `Bean.sleep()` is more power-efficient than Arduino `sleep()` because it puts the ATmega into a low-power mode known as "power-down". This disables the ATmega's internal timers, so functions like `millis()` will not track time elapsed during `Bean.sleep()`.
   *
   *  The ATmega can take up to 7 ms to wake from `Bean.sleep()`. If you are looking for more precise timings, please consider using [delay()](https://www.arduino.cc/en/Reference/Delay) or [delayMicroseconds()](https://www.arduino.cc/en/Reference/DelayMicroseconds).
   *
   *  For more information on low-power mode on the ATmega328, check out this [Sparkfun tutorial](https://www.sparkfun.com/tutorials/309).
   *
   *  @param duration_ms The duration to sleep for, in milliseconds
   *
   *  # Examples
   *
   *  This example turns on Bean's LED, sleeps for one second, turns off Bean's LED and sleeps for four more seconds, then repeats:
   *
   *  @include sleep/sleep.ino
   */
  void sleep(uint32_t duration_ms);

  /**
   *  Needs docs
   */
  void keepAwake(bool enable);

  /**
   *  Enable or disable wake on connect. By default, Bean does not wake up when a BLE client connects.
   *
   *  @param enable true to enable wake on connect, false to disable
   *
   *  # Examples
   *
   *  This example wakes Bean when a client connects, waits for the client to disconnect, and sleeps on disconnect:
   *
   *  @include sleep/enableWakeOnConnect.ino
   */
  void enableWakeOnConnect(bool enable);
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
