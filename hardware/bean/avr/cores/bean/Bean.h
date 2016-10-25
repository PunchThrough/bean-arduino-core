#ifndef BEAN_BEAN_BEAN_H
#define BEAN_BEAN_BEAN_H
#include "BeanSerialTransport.h"
#include "BeanHID.h"
#include "BeanMidi.h"
#include "BeanAncs.h"
#include "bma250.h"

/*
 *  An accelerometer interrupt type, they are as follows:
 *  FLAT_EVENT - triggers when the accelerometer is lying flat on a surface
 *  ORIENT_EVENT - triggers when the accelerometer is NOT lying flat on a surface, but is tilted *  in any direction
 *  SINGLE_TAP_EVENT - triggers when the accelerometer is tapped once
 *  DOUBLE_TAP_EVENT - triggers when the accelerometer is tapped twice
 *  ANY_MOTION_EVENT - triggers when the accelerometer experiences any change in motion
 *  HIGH_G_EVENT - triggers when the accelerometer experiences a velocity event higher than it's *  sensitivity
 *  LOW_G_EVENT - triggers when the accelerometer is in free fall or experiences no gravitational *  pull
 */
typedef enum AccelEventTypes {
  FLAT_EVENT = 0x80,
  ORIENT_EVENT = 0x40,
  SINGLE_TAP_EVENT = 0x20,
  DOUBLE_TAP_EVENT = 0x10,
  ANY_MOTION_EVENT = 0x04,
  HIGH_G_EVENT = 0x02,
  LOW_G_EVENT = 0x01
};

typedef enum AdvertisementDataTypes {
  GAP_ADTYPE_FLAGS                        =  0x01,  //  Discovery Mode: @ref GAP_ADTYPE_FLAGS_MODES
  GAP_ADTYPE_16BIT_MORE                   =  0x02,  //  Service: More 16-bit UUIDs available
  GAP_ADTYPE_16BIT_COMPLETE               =  0x03,  //  Service: Complete list of 16-bit UUIDs
  GAP_ADTYPE_32BIT_MORE                   =  0x04,  //  Service: More 32-bit UUIDs available
  GAP_ADTYPE_32BIT_COMPLETE               =  0x05,  //  Service: Complete list of 32-bit UUIDs
  GAP_ADTYPE_128BIT_MORE                  =  0x06,  //  Service: More 128-bit UUIDs available
  GAP_ADTYPE_128BIT_COMPLETE              =  0x07,  //  Service: Complete list of 128-bit UUIDs
  GAP_ADTYPE_LOCAL_NAME_SHORT             =  0x08,  //  Shortened local name
  GAP_ADTYPE_LOCAL_NAME_COMPLETE          =  0x09,  //  Complete local name
  GAP_ADTYPE_POWER_LEVEL                  =  0x0A,  //  TX Power Level: 0xXX: -127 to +127 dBm
  GAP_ADTYPE_OOB_CLASS_OF_DEVICE          =  0x0D,  //  Simple Pairing OOB Tag: Class
                                                    //  of device (3 octets)
  GAP_ADTYPE_OOB_SIMPLE_PAIRING_HASHC     =  0x0E,  //  Simple Pairing OOB Tag: Simple Pairing
                                                    //  Hash C (16 octets)
  GAP_ADTYPE_OOB_SIMPLE_PAIRING_RANDR     =  0x0F,  //  Simple Pairing OOB Tag: Simple Pairing
                                                    //  Randomizer R (16 octets)
  GAP_ADTYPE_SM_TK                        =  0x10,  //  Security Manager TK Value
  GAP_ADTYPE_SM_OOB_FLAG                  =  0x11,  //  Secutiry Manager OOB Flags
  GAP_ADTYPE_SLAVE_CONN_INTERVAL_RANGE    =  0x12,  //  Min and Max values of the connection
                                                    //  interval
                                                    //  (2 octets Min, 2 octets Max) (0xFFFF
                                                    //  indicates no conn interval min or max)
  GAP_ADTYPE_SIGNED_DATA                  =  0x13,  //  Signed Data field
  GAP_ADTYPE_SERVICES_LIST_16BIT          =  0x14,  //  Service Solicitation:
                                                    //  list of 16-bit Service UUIDs
  GAP_ADTYPE_SERVICES_LIST_128BIT         =  0x15,  //  Service Solicitation:
                                                    //  list of 128-bit Service UUIDs
  GAP_ADTYPE_SERVICE_DATA                 =  0x16,  //  Service Data
  GAP_ADTYPE_APPEARANCE                   =  0x19,  //  Appearance
  GAP_ADTYPE_MANUFACTURER_SPECIFIC        =  0xFF,  //  Manufacturer Specific Data:
                                                    //  first 2 octets contain
                                                    //  the Company Identifier Code
                                                    //  followed by the additional
                                                    //  manufacturer specific data
};

typedef enum AdvertisementType {
  GAP_ADTYPE_FLAGS_LIMITED                =  0x01,  //  Discovery Mode: LE Limited Discoverable Mode
  GAP_ADTYPE_FLAGS_GENERAL                =  0x02,  //  Discovery Mode: LE General Discoverable Mode
  GAP_ADTYPE_FLAGS_BREDR_NOT_SUPPORTED    =  0x04,  //  Discovery Mode: BR/EDR Not Supported
};


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
 *  Currently enabled advertisements in the rotating advertisement controller.
 */
typedef ADV_SWITCH_ENABLED_T BluetoothServices;

/**
 *  Data returned by the observer role.
 */
typedef OBSERVER_INFO_MESSAGE_T ObserverAdvertisementInfo;

class BeanClass {
 public:
  /****************************************************************************/
  /** @name Accelerometer
   *  Read acceleration values and configure the accelerometer's sensitivity.
   */
  ///@{

  /**
   *  Enable accelerometer interrupts
   *
   *  @param accepts an event of type AccelEventTypes
   */
  void enableMotionEvent(AccelEventTypes events);

  /**
   *  Disables all currently enabled accelerometer interrupts
   */
  void disableMotionEvents();

  /**
   *  Checks to see if a particular acclerometer interrupt has occured.  If the event occurs it sets a flag that can only be cleared by reading this function.
   *
   *  @param accepts an event of type AccelEventTypes
   *
   *  # Examples
   *
   *  This example sets off police lights on the Bean if motion is detected:
   *
   *  @include accelerometer/checkMotionEvent.ino
   */
  bool checkMotionEvent(AccelEventTypes events);

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
   *  Low level function for writing directly to the accelerometers registers.
   *
   *  @param reg the register to write to
   *  @param value the value to write to the register
   */
  void accelRegisterWrite(uint8_t reg, uint8_t value);

  /**
   *  Low level function for reading the accelerometers register directly
   *
   *  @param reg the register to read
   *  @param length the number of bytes to read starting at that register
   *  @param value a pointer to a user supplied array to fill with values
   *  @return the number of bytes actually read
   *
   *  # Examples
   *
   *  This example uses a Bean as a motion alarm by reading/writing to the accelerometer register directly:
   *
   *  @include accelerometer/accelRegisterWrite.ino
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
   *
   *  # Examples
   *
   *  This example prints out the acceleration values associated with high and low g-force values:
   *
   *  @include accelerometer/getAccelerationRange.ino
   */
  void setAccelerationRange(uint8_t range);

  /**
   *  Set power mode of the accelerometer
   *
   *  @param mode the power mode of the accelerometer
   *
   *  Power modes: Normal mode:     0x00    (~140uA)
   *               Suspend mode:    0x80    (~0.5uA)
   *               Low Power 10ms:  0x54    (~16.4uA)
   *               Low Power 100ms: 0x5A    (~2.3uA)
   *               Low Power 1s:    0x5E    (~0.7uA)
   */
  void setAccelerometerPowerMode(uint8_t mode);

  /**
   *  Get power mode of the accelerometer
   *
   *  @return the power mode of the accelerometer
   *
   *  Power modes: Normal mode:     0x00    (~140uA)
   *               Suspend mode:    0x80    (~0.5uA)
   *               Low Power 10ms:  0x54    (~16.4uA)
   *               Low Power 100ms: 0x5A    (~2.3uA)
   *               Low Power 1s:    0x5E    (~0.7uA)
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
   *  @param red the intensity of the red LED. 0 is off and 255 is on.
   *
   *  @param green the intensity of the green LED. 0 is off and 255 is on.
   *
   *  @param blue the intensity of the blue LED. 0 is off and 255 is on.
   *
   *  # Examples
   *
   *  This example sets the LED to white (all channels on, full intensity):
   *
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
   *
   *  @include led/getLed.ino
   */
  LedReading getLed(void);

  /**
   *  Get intensity of the red channel of the Bean RGB LED. 0 is off and 255 is on.
   *
   *  @return 0 for off and 255 for on.
   */
  uint8_t getLedRed(void);

  /**
   *  Get intensity of the green channel of the Bean RGB LED. 0 is off and 255 is on.
   *
   *  @return 0 for off and 255 for on.
   */
  uint8_t getLedGreen(void);

  /**
   *  Get intensity of the blue channel of the Bean RGB LED. 0 is off and 255 is on.
   *
   *  @return 0 for off and 255 for on.
   */
  uint8_t getLedBlue(void);

  /**
   *  Set intensity of the red channel of the Bean RGB LED. 0 is off and 255 is on.
   *
   *  @param intensity the intensity of the red LED. 0 is off and 255 is on.
   */
  void setLedRed(uint8_t intensity);

  /**
   *  Set intensity of the green channel of the Bean RGB LED. 0 is off and 255 is on.
   *
   *  @param intensity the intensity of the green LED. 0 is off and 255 is on.
   */
  void setLedGreen(uint8_t intensity);

  /**
   *  Set intensity of the blue channel of the Bean RGB LED. 0 is off and 255 is on.
   *
   *  @param intensity the intensity of the blue LED. 0 is off and 255 is on.
   */
  void setLedBlue(uint8_t intensity);
  ///@}

  /****************************************************************************/
  /** @name Observer
   *  Functions related to Observer mode.  Observer role allows the Bean to listen for advertisements.
   *  As a peripheral, the Bean cannot send scan response requests, but it can listen for undirected advertisements.
   *  Paired with the custom advertising role, you can have two Beans interact with each other.  This role works even when the Bean is connected.
   */
  ///@{

  /**
   *  Listens for advertisements and fills an ObserverAdvertisementInfo message with the first advertisement it sees
   *  This function blocks until it receives a message or times out.
   *
   *  @param message a pointer to a message object supplied by the user
   *  @param timeout how long in milliseconds to listen for an advertisement
   *  @return -1 if there was a failure, 1 if there was a success
   *
   *  # Examples
   *
   *  This sketch (observer.ino) demonstrates Bean's observer role functionality.
   *
   *  When a Bean running this sketch is near a Bean running advertiser.ino, it will change its LED to reflect the state of the LED on the advertising Bean.
   *
   *  @include observer/observer.ino
   */
  int getObserverMessage(ObserverAdvertisementInfo *message, unsigned long timeout);
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
   *
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
   *
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
   *
   *  @include scratchChars/readScratchData.ino
   */
  ScratchData readScratchData(uint8_t bank);

  /**
   *  Read a 32-bit (four-byte) value from a scratch characteristic.
   *
   *  @param bank         The index of the source scratch char: `1`, `2`, `3`, `4`, or `5`
   *
   *  @return             The number of the scratch characteristic
   *
   *  # Examples
   *
   *  This example writes a value to and reads a value from a scratch characteristic:
   *
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
   *  Enable or disable keep-awake mode.
   *
   *  By default, the Bean radio sleeps frequently to conserve power. Enabling keep-awake forces the LBM into wake mode and decreases latency between the LBM313 and the ATmega.
   *
   *  This may be useful if you are having trouble with latency between an event and a Bluetooth transmission: for example, to decrease the time between Bean reading a pin change event and sending a Bluetooth message.
   *
   *  Enabling keep-awake may signficantly decrease battery life. Use with caution.
   *
   *  @param true to enable keep-awake, false to disable
   *
   *  # Examples
   *
   *  This example keeps the Bean awake to reduce latency between reading a pinstate and transmitting Bluetooth information:
   *
   *  @include awake/keepAwake.ino
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
   *
   *  Bean makes itself visible to BLE Central devices by broadcasting BLE advertising packets. If advertising is disabled, Central devices will not be able to find or connect to Bean.
   *
   *  Advertising is automatically enabled when Bean is powered on or resets. However, uploading a sketch that disables advertising indefinitely may make it impossible to connect to Bean. If this happens to you, see [this guide (TODO: ADD GUIDE + LINK)](#) to clear the sketch stored on Bean.
   */
  ///@{

  /**
   *  Set the advertising name of the Bean. BLE advertising names are truncated at 20 bytes.
   *
   *  @param s The name to be advertised
   *
   *  # Examples
   *
   *  This example changes Bean's name based on its temperature:
   *
   *  @include advertising/setBeanName.ino
   */
  void setBeanName(const String &s);

  /**
   *  Read the currently-advertised name of the Bean.
   *
   *  @return The Bean name as a char array, null-terminated
   *
   *  # Examples
   *
   *  This example prints Bean's name to Virtual Serial every two seconds:
   *
   *  @include advertising/getBeanName.ino
   */
  const char *getBeanName(void);

  /**
   *  Sets the Beans advertisement interval.  This is useful if you are trying to optimize battery life at the exense of advertisement rates
   *  and can also be useful for increasing beacon advertisement rates.
   *  @param interval_ms length of advertisement interval in milliseconds.  Minimum of BEAN_MIN_ADVERTISING_INT_MS and max of BEAN_MAX_ADVERTISING_INT_MS
   */
  void setAdvertisingInterval(uint16_t interval_ms);

  /**
   *  Enable or disable BLE advertising for a specific duration.
   *
   *  Advertising is automatically enabled when Bean is powered on or resets. Configuration changes made by calling this method are **not** stored in non-volatile memory.
   *
   *  @param enable true to enable advertising, false to disable
   *  @param timer the duration to enable/disable advertising, in milliseconds
   *
   *  # Examples
   *
   *  This example disables advertising for 10 seconds when digital pin 0 is pulled low:
   *
   *  @include advertising/enableAdvertisingInterval.ino
   */
  void enableAdvertising(bool enable, uint32_t timer);

  /**
   *  Enable or disable BLE advertising.
   *
   *  Advertising is automatically enabled when Bean is powered on or resets. Configuration changes made by calling this method are **not** stored in non-volatile memory.
   *
   *  @param enable true to enable advertising, false to disable
   *
   *  # Examples
   *
   *  This example disables advertising when digital pin 0 is pulled low and enables it otherwise:
   *
   *  @include advertising/enableAdvertising.ino
   */
  void enableAdvertising(bool enable);

  /**
   *  Check whether the Bean is currently advertising.
   *
   *  @return true if Bean is advertising, false if Bean is not advertising
   *
   *  # Examples
   *
   *  This example toggles Bean's advertising every 15 seconds and indicates the current advertising status with the LED:
   *
   *  @include advertising/getAdvertisingState.ino
   *
   */
  bool getAdvertisingState(void);

  /**
   *  Enables custom advertisement.  The Bean will enter a rotating advertisement mode where it will advertise as a bean for a few moments then advertise whatever is in the custom advertisement packet and back again
   */
  void enableCustom(void);

  /**
   *  Disables custom advertisement.
   */
  void disableCustom(void);

  /**
   *  Sets the custom advertisement packet.  The max length is 31 bytes.
   *  The first 3 bytes specify the advertisement mode.  They take the form 0x2, GAP_ADTYPE_FLAGS, any sum of advertisement types (as defined by AdvertisementType)
   *  All following data are up to the user to define and follow the pattern of [length, AdvertisementDataTypes, data1, data2, ...] where length includes the number of data plus 1 (for the AdvertisementDataTypes)
   *  The data can be chained together into the single buffer up to the maxiumum length.
   *  For example:
   *  [0x02, GAP_ADTYPE_FLAGS, GAP_ADTYPE_FLAGS_BREDR_NOT_SUPPORTED + GAP_ADTYPE_FLAGS_GENERAL, 0x02, GAP_ADTYPE_MANUFACTURER_SPECIFIC, 42, 0x02, GAP_ADTYPE_POWER_LEVEL, 10, ...]
   *
   *  @param buf a buffer full of data to advertise
   *  @param len the length of the buffer
   *
   *  # Examples
   *
   *  This sketch (advertiser.ino) will enable a custom advertisement for the Bean. It will broadcast the following packet:
   *
   *  `{0x02, GAP_ADTYPE_FLAGS (0x01), GAP_ADTYPE_FLAGS_GENERAL | GAP_ADTYPE_FLAGS_BREDR_NOT_SUPPORTED (0x06), 0x03, GAP_ADTYPE_MANUFACTURER_SPECIFIC (0xFF), 0xAC, 0x0n}`
   *
   *  When pin 0 is pulled to ground, Bean will toggle the green LED.  It will also set the last byte in the advertisement packet to reflect the LED's on state.
   *  When used in conjunction with observer.ino, the observer Bean will see this packet and will turn its light on.
   *
   *  @include observer/advertiser.ino
   */
  void setCustomAdvertisement(uint8_t *buf, int len);
  ///@}


  /****************************************************************************/
  /** @name iBeacon
   *  Let your Bean act as an <a href="https://developer.apple.com/ibeacon/">iBeacon</a>, a way to convey real-world location to iOS devices.
   *
   *  Bean supports a limited subset of UUIDs available to iBeacon devices. A Bean iBeacon UUID is made up of 16 user-configurable bits and 112 preconfigured bits, where `xx` represents a user-configurable byte:
   *
   *  `A495xxxx-C5B1-4B44-B512-1370F02D74DE`
   *
   *  Major and minor IDs are used to identify the iBeacon with greater accuracy than with the UUID alone, and are both unsigned integers between 0 and 66535
   *
   *  Using iBeacon features will, by default, write to Bean's NVRAM. The NVRAM has a limited number of writes. Use `enableConfigSave` to store settings temporarily and conserve NVRAM writes.
   */
  ///@{

  /**
   *  Works very similarly to setBeaconEnable.   The primary difference being that enableiBeacon adds the beacon advertisement to a rotating adverisement instead of overrwriting the current standard Bean advertisement.  Parameters are still set with the setBeaconParameters function.
   */
  void enableiBeacon(void);

  /**
   *  Configure Bean's iBeacon UUID, major ID, and minor ID. Each of these parameters, **including UUID**, takes 16-bit unsigned values. For more information on UUID, major, and minor values, see [this iBeacon FAQ](https://support.kontakt.io/hc/en-gb/articles/201620741-iBeacon-Parameters-UUID-Major-and-Minor).
   *
   *  @param uuid The 16-bit value used to set part of the iBeacon UUID. For example: Passing `0xABCD` to `uuid` will set the Bean's UUID to `A495ABCD-C5B1-4B44-B512-1370F02D74DE`.
   *  @param major_id The major ID of the iBeacon
   *  @param major_id The minor ID of the iBeacon
   *
   *  # Examples
   *
   *  This example configures iBeacon and uses the Bean only to advertise and transmit iBeacon data:
   *
   *  @include beacon/setBeaconParameters.ino
   */
  void setBeaconParameters(uint16_t uuid, uint16_t major_id, uint16_t minor_id);

  /**
   *  Enable or disable iBeacon functionality.
   *
   *  @param beaconEnable true to enable iBeacon, false to disable
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
   *
   *  # Examples
   *
   *  This example will display a Bean's respective current battery level and voltage every 5 seconds:
   *
   *  @include battery/getBatteryLevel.ino
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
   *
   *  # Examples
   *
   *  This example changes the color of the LED based on the temperature reading of the Bean:
   *
   *  @include temperature/getTemperature.ino
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
   *
   *  # Examples
   *
   *  This example blinks the LED on the Bean green if connected, red if disconnected:
   *
   *  @include connection/getConnectionState.ino
   */
  bool getConnectionState(void);
  ///@}


  /***************************************************************************/
  /** @name Bluetooth Services
   *  Functions dealing with Bluetooth service configurations and pairing.
   */
  ///@{

  /**
   *  Returns a struct of all of the currently services and whether or not they are enabled.
   */
  BluetoothServices getServices(void);

  /**
   *  Sets services for the Bean to use (NOTE: disabling the standard service will no longer allow the Bean to connect to the Bean Loader)
   *
   *  @param services the services to change
   */
  void setServices(BluetoothServices services);

  /**
   *  Resets services leaving only the primary standard Bean service advertising.
   */
  void resetServices(void);

  /**
   *  Sets the pairing PIN for a secure PIN-based connection. The default PIN is 000000.
   *
   *  Calling this function enables the pairing PIN mode.  Disable it with `enablePairingPin(false)`.
   *
   *  If you wish to use this with HID or ANCS, set the PIN after enabling either of the profiles.
   *
   *  After pairing, your device and Bean will be bonded. In order to un-bond, you will have to manually disconnect from Bean in your device's settings.
   *
   *  The PIN will be saved to nonvolatile memory unless `enableConfigSave(false)` is called.
   *
   *  @param pin the pairing PIN to set, from 000000 to 999999
   *
   *  # Examples
   *
   *  This example sets a pairing PIN of 123456 for future connections:
   *
   *  @include pairing/setPairingPin.ino
   */
  void setPairingPin(uint32_t pin);

  /**
   *  Enables or disables pairing PIN functionality.
   *
   *  The default PIN is 000000. This can be changed using `setPairingPin`.
   *
   *  @param true to enable, false to disable
   */
  void enablePairingPin(bool enable);
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
   *
   *  # Examples
   *
   *  This example enables settings that will only be used temporarily because enableConfigSave is used:
   *
   *  @include config/enableConfigSave.ino
   */
  void enableConfigSave(bool enableSave);


  /**
   *  Performs a hard reset on the bluetooth module.
   *
   *  WARNING: Advanced function. The misuse of this method can cause the Bean
   *           to repeatedly reboot, causing it to be unconnectable.
   */
  void restartBluetooth(void);

  ///@}


  BeanClass() {}

 private:
  /**
   *  Send a message from the ATmega to the CC2540 asking it to wake up the ATmega at a given time. The CC2540 might not receive the message, so it's important to check the return value of this method.
   *
   *  @param duration_ms The duration to sleep for, in milliseconds
   *
   *  @return true if the message was acknowledged by the CC2540 successfully, false if the message was not acknowledged and should be sent again
   */
  bool attemptSleep(uint32_t duration_ms);

  /**
   *  Needs docs
   */
  int16_t convertAcceleration(uint8_t high_byte, uint8_t low_byte);

  /**
   *  Set interrupts and power mode of the accelerometer
   *
   *  @param interrupts number of accelerometer interrupts
   *  @param power_mode power mode of the accelerometer
   *
   *  Power modes: Normal mode:     0x00    (~140uA)
   *               Suspend mode:    0x80    (~0.5uA)
   *               Low Power 10ms:  0x54    (~16.4uA)
   *               Low Power 100ms: 0x5A    (~2.3uA)
   *               Low Power 1s:    0x5E    (~0.7uA)
   */
  void accelerometerConfig(uint16_t interrupts, uint8_t power_mode);

  /**
   *  Enables Bean to awaken when acceleration is detected
   *
   *  @param sources set wake interrupt; awake until latch time expires (default 250 ms)
   *
   *  Wake modes: Wake for any event     0xF7
   *              Wake when flat         0x80
   *              Wake when oriented     0x40
   *              Wake on single tap     0x20
   *              Wake on double-tap     0x10
   *              Wake on any motion     0x04
   *              Wake on high g-force   0x02
   *              Wake on low g-force    0x01
   */
  void enableWakeOnAccelerometer(uint8_t sources);

  /**
   *  Get the accelerometer interrupts
   *
   *  @return the accelerometer interrupts
   */
  uint8_t checkAccelInterrupts();
};

/**
 *  Needs docs
 *  Library of Bean-specific functions to run sketches on ATMega386p
 */
extern BeanClass Bean;

#endif
