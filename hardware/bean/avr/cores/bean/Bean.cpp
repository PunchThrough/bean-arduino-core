#include "Bean.h"
#include "BeanHID.h"
#include "Arduino.h"
#include <avr/sleep.h>
#include <avr/interrupt.h>
#include <avr/io.h>
#include <avr/wdt.h>
#include <applicationMessageHeaders/AppMessages.h>
#include "wiring_private.h"

#ifndef sleep_bod_disable()  // not included in Arduino AVR toolset
#define sleep_bod_disable()                         \
  do {                                              \
    uint8_t tempreg;                                \
    __asm__ __volatile__(                           \
        "in %[tempreg], %[mcucr]"                   \
        "\n\t"                                      \
        "ori %[tempreg], %[bods_bodse]"             \
        "\n\t"                                      \
        "out %[mcucr], %[tempreg]"                  \
        "\n\t"                                      \
        "andi %[tempreg], %[not_bodse]"             \
        "\n\t"                                      \
        "out %[mcucr], %[tempreg]"                  \
        : [tempreg] "=&d"(tempreg)                  \
        : [mcucr] "I" _SFR_IO_ADDR(MCUCR),          \
          [bods_bodse] "i"(_BV(BODS) | _BV(BODSE)), \
          [not_bodse] "i"(~_BV(BODSE)));            \
  } while (0)
#endif

#define MAX_SCRATCH_SIZE (20)
#define NUM_BEAN_PINS 7

BeanClass Bean;

static void wakeUp(void) {
  // Do nothing.
  // This function is called as an interrupt purely to wake
  // us up.
  return;
}

#define MAX_SLEEP_POLL (30)
#define MAX_DELAY (30000)
#define MIN_SLEEP_TIME (10)

void BeanClass::keepAwake(bool enable) {
  if (enable) {
    Serial.BTConfigUartSleep(UART_SLEEP_NEVER);
  } else {
    Serial.BTConfigUartSleep(UART_SLEEP_NORMAL);
  }
}

bool BeanClass::attemptSleep(uint32_t duration_ms) {
  // ensure that our interrupt line is an input
  DDRD &= ~(_BV(3));

  bool sleepLineSet = false;
  uint8_t pollCount = 0;

  // Send the sleep message to the TI and wait for it to
  // finish sending.
  Serial.sleep(duration_ms);
  Serial.flush();

  while (sleepLineSet == false && pollCount++ < MAX_SLEEP_POLL) {
    delay(1);
    if ((PIND & _BV(3)) > 0) {
      sleepLineSet = true;
    }
  }

  return sleepLineSet;
}

void BeanClass::enableConfigSave(bool enableSave) {
  Serial.BTSetEnableConfigSave(enableSave);
}

void BeanClass::restartBluetooth(void) {
  Serial.BTRestart();
}

void BeanClass::sleep(uint32_t duration_ms) {
  // ensure that our interrupt line is an input
  DDRD &= ~(_BV(3));

  Serial.BTConfigUartSleep(UART_SLEEP_NORMAL);

  // There's no point in sleeping if the duration is <= 10ms
  if (duration_ms < MIN_SLEEP_TIME) {
    delay(duration_ms);
    return;
  }

  // poll and wait for interrupt line to go HIGH (sleep)

  // attempt sleep, if it fails, waited a total of 10ms
  bool sleeping = false;

  sleeping = attemptSleep(duration_ms);

  if (!sleeping && duration_ms > MAX_DELAY) {
    // keep trying until the end of delay period
    while (duration_ms > 0 && false == sleeping) {
      duration_ms =
          (duration_ms >= MAX_SLEEP_POLL) ? duration_ms - MAX_SLEEP_POLL : 0;
      sleeping = attemptSleep(duration_ms);
    }
  } else if (!sleeping && duration_ms > MAX_SLEEP_POLL) {
    // take out the time we've already delayed
    delay(duration_ms - MAX_SLEEP_POLL);
    sleeping = false;
  }

  // if we never slept, don't set interrupts
  if (sleeping == false) {
    return;
  }

  // set our interrupt pin to input:
  const int interruptNum = 1;

  bool adc_was_set = bit_is_set(ADCSRA, ADEN);
  if (adc_was_set) {
    // disable ADC
    ADCSRA &= ~(_BV(ADEN));
  }

  bool ac_was_set = bit_is_set(ACSR, ACD);
  if (ac_was_set) {
    // disable ADC
    ACSR &= ~(_BV(ACD));
  }

  // Details on how to manage sleep mode with AVR gotten from the avr-libc
  // manual, found here:
  // http://www.nongnu.org/avr-libc/user-manual/group__avr__sleep.html
  // (block quote below)

  // Note that unless your purpose is to completely lock the CPU (until a
  // hardware reset), interrupts need to be enabled before going to sleep.

  // As the sleep_mode() macro might cause race conditions in some situations,
  // the individual steps of manipulating the sleep enable (SE) bit, and
  // actually issuing the SLEEP instruction, are provided in the macros
  // sleep_enable(), sleep_disable(), and sleep_cpu(). This also allows for
  // test-and-sleep scenarios that take care of not missing the interrupt
  // that will awake the device from sleep.

  // Example:

  //     #include <avr/interrupt.h>
  //     #include <avr/sleep.h>

  //     ...
  //       set_sleep_mode(<mode>);
  //       cli();
  //       if (some_condition)
  //       {
  //         sleep_enable();
  //         sei();
  //         sleep_cpu();
  //         sleep_disable();
  //       }
  //       sei();
  // This sequence ensures an atomic test of some_condition with interrupts
  // being disabled. If the condition is met, sleep mode will be prepared,
  // and the SLEEP instruction will be scheduled immediately after an SEI
  // instruction. As the intruction right after the SEI is guaranteed to be
  // executed before an interrupt could trigger, it is sure the device will
  // really be put to sleep.

  // Some devices have the ability to disable the Brown Out Detector (BOD)
  // before going to sleep. This will also reduce power while sleeping.
  // If the specific AVR device has this ability then an additional macro
  // is defined: sleep_bod_disable(). This macro generates inlined assembly
  // code that will correctly implement the timed sequence for disabling the
  // BOD before sleeping. However, there is a limited number of cycles after
  // the BOD has been disabled that the device can be put into sleep mode,
  // otherwise the BOD will not truly be disabled. Recommended practice is
  // to disable the BOD (sleep_bod_disable()), set the interrupts (sei()),
  // and then put the device to sleep (sleep_cpu()), like so:

  //     #include <avr/interrupt.h>
  //     #include <avr/sleep.h>

  //     ...
  //       set_sleep_mode(<mode>);
  //       cli();
  //       if (some_condition)
  //       {
  //         sleep_enable();
  //         sleep_bod_disable();
  //         sei();
  //         sleep_cpu();
  //         sleep_disable();
  //       }
  //       sei();

  /* In the function call attachInterrupt(A, B, C)
  * A   can be either 0 or 1 for interrupts on pin 2 or 3.
  *
  * B   Name of a function you want to execute at interrupt for A.
  *
  * C   Trigger mode of the interrupt pin. can be:
  *             LOW        a low level triggers
  *             CHANGE     a change in level triggers
  *             RISING     a rising edge of a level triggers
  *             FALLING    a falling edge of a level triggers
  *
  * In all but the IDLE sleep modes only LOW can be used.
  */
  attachInterrupt(interruptNum, wakeUp, LOW);
  set_sleep_mode(SLEEP_MODE_PWR_DOWN);
  cli();
  if (bit_is_set(PIND, 3)) {
    sleep_enable();
    sleep_bod_disable();
    sei();
    sleep_cpu();
    sleep_disable();
  }
  sei();

  detachInterrupt(interruptNum);

  if (adc_was_set) {
    // re-enable adc
    ADCSRA |= _BV(ADEN);
  }

  if (ac_was_set) {
    // re-enable analog compareter
    ACSR |= _BV(ACD);
  }
}

void BeanClass::setAdvertisingInterval(uint16_t interval_ms) {
  Serial.BTSetAdvertisingInterval(interval_ms);
}

void BeanClass::enableAdvertising(bool enable, uint32_t timer) {
  Serial.BTSetAdvertisingOnOff(enable, timer);
}

void BeanClass::enableAdvertising(bool enable) {
  Serial.BTSetAdvertisingOnOff(enable, 0);
}

bool BeanClass::getConnectionState(void) {
  BT_STATES_T btStates;
  if (Serial.BTGetStates(&btStates) == 0) {
    return (bool)btStates.conn_state;
  }
  return 0;
}

bool BeanClass::getAdvertisingState(void) {
  BT_STATES_T btStates;
  if (Serial.BTGetStates(&btStates) == 0) {
    return (bool)btStates.adv_state;
  }
  return 0;
}

int8_t BeanClass::getTemperature(void) {
  int8_t temp = 0;

  Serial.temperatureRead(&temp);

  return temp;
}

uint8_t BeanClass::getBatteryLevel(void) {
  uint8_t level = 0;

  Serial.batteryRead(&level);

  return level;
}

uint16_t BeanClass::getBatteryVoltage(void) {
  uint32_t actualVoltage = 0;
  uint8_t level = 0;

  Serial.batteryRead(&level);

  // This may not return accurate readings.  Conversion is subject to change.
  // The conversion function from voltage to level is as follows:
  //  f(x) = x * (63.53) - 124.26
  //
  //  solve for voltage and you get the function below, including fixed-point
  //  scaling
  //  with two decimal points of precision
  actualVoltage =
      (((uint32_t)100 * (uint32_t)level + (uint32_t)12426) * (uint32_t)100) /
      6353;

  return (uint16_t)actualVoltage;
}

void BeanClass::accelRegisterWrite(uint8_t reg, uint8_t value) {
  Serial.accelRegisterWrite(reg, value);
}

int BeanClass::accelRegisterRead(uint8_t reg, uint8_t length, uint8_t *value) {
  return Serial.accelRegisterRead(reg, length, value);
}

void BeanClass::setAccelerometerPowerMode(uint8_t mode) {
  Serial.accelRegisterWrite(REG_POWER_MODE_X11, mode);
}

uint8_t BeanClass::getAccelerometerPowerMode() {
  uint8_t value;
  Serial.accelRegisterRead(REG_POWER_MODE_X11, 1, &value);
  return value;
}

void BeanClass::enableWakeOnAccelerometer(uint8_t sources) {
  Serial.accelRegisterWrite(REG_LATCH_CFG_X21, VALUE_TEMPORARY_250MS);
  Serial.accelRegisterWrite(REG_INT_MAPPING_X19, sources);
  Serial.wakeOnAccel(1);
}

uint8_t BeanClass::getAccelerationRange(void) {
  uint8_t value;
  Serial.accelRegisterRead(REG_G_SETTING, 1, &value);
  return value;
}

void BeanClass::setAccelerationRange(uint8_t range) {
  Serial.accelRegisterWrite(REG_G_SETTING, range);
}

int16_t BeanClass::getAccelerationX(void) {
  ACC_READING_T reading;
  Serial.accelRead(&reading);
  return reading.xAxis;
}

int16_t BeanClass::getAccelerationY(void) {
  ACC_READING_T reading;
  Serial.accelRead(&reading);
  return reading.yAxis;
}

int16_t BeanClass::getAccelerationZ(void) {
  ACC_READING_T reading;
  Serial.accelRead(&reading);
  return reading.zAxis;
}

ACC_READING_T BeanClass::getAcceleration(void) {
  ACC_READING_T reading;
  Serial.accelRead(&reading);
  return reading;
}

static uint8_t enabledEvents = 0x00;
static uint8_t triggeredEvents = 0x00;
void BeanClass::enableMotionEvent(AccelEventTypes events) {
  uint16_t enableRegister = 0x0000;
  uint8_t wakeRegister = 0x00;

  enabledEvents |= events;

  if (enabledEvents & FLAT_EVENT) {
    enableRegister |= ENABLE_FLAT_INT;
    wakeRegister |= WAKE_FLAT_INT;
  }

  if (enabledEvents & ORIENT_EVENT) {
    enableRegister |= ENABLE_ORIENT_INT;
    wakeRegister |= WAKE_ORIENT_INT;
  }

  if (enabledEvents & SINGLE_TAP_EVENT) {
    enableRegister |= ENABLE_SINGLE_TAP_INT;
    wakeRegister |= WAKE_SINGLE_TAP_INT;
  }

  if (enabledEvents & DOUBLE_TAP_EVENT) {
    enableRegister |= ENABLE_DOUBLE_TAP_INT;
    wakeRegister |= WAKE_DOUBLE_TAP_INT;
  }

  if (enabledEvents & ANY_MOTION_EVENT) {
    enableRegister |= ENABLE_ANY_MOTION_INT;
    wakeRegister |= WAKE_ANY_MOTION_INT;
  }

  if (enabledEvents & HIGH_G_EVENT) {
    enableRegister |=
        (ENABLE_HIGH_G_Z_INT | ENABLE_HIGH_G_Y_INT | ENABLE_HIGH_G_X_INT);
    wakeRegister |= WAKE_HIGH_G_INT;
  }

  if (enabledEvents & LOW_G_EVENT) {
    enableRegister |= ENABLE_LOW_G_INT;
    wakeRegister |= WAKE_LOW_G_INT;
  }

  // Clear triggered event flags for newly enabled events
  triggeredEvents &= ~events;
  accelerometerConfig(enableRegister, VALUE_LOW_POWER_10MS);
  enableWakeOnAccelerometer(wakeRegister);
}

void BeanClass::disableMotionEvents() {
  enabledEvents = 0;
  accelerometerConfig(0, VALUE_LOW_POWER_1S);
}

// This function returns true if any one of the "events" param had been
// triggered
// It clears all corresponding "events" flags
bool BeanClass::checkMotionEvent(AccelEventTypes events) {
  triggeredEvents |= checkAccelInterrupts();

  bool eventOccurred = (triggeredEvents & events) ? true : false;
  triggeredEvents &= ~events;

  return eventOccurred;
}

void BeanClass::accelerometerConfig(uint16_t interrupts, uint8_t power_mode) {
  Serial.accelRegisterWrite(REG_POWER_MODE_X11, power_mode);
  Serial.accelRegisterWrite(REG_LATCH_CFG_X21, VALUE_LATCHED);
  Serial.accelRegisterWrite(REG_INT_SETTING_X16, (uint8_t)(interrupts >> 8));
  Serial.accelRegisterWrite(REG_INT_SETTING_X17, (uint8_t)(interrupts & 0xFF));
}

uint8_t BeanClass::checkAccelInterrupts() {
  uint8_t value;
  uint8_t latch_cfg;
  Serial.accelRegisterRead(REG_INT_STATUS_X09, 2, &value);
  Serial.accelRegisterRead(REG_LATCH_CFG_X21, 1, &latch_cfg);
  latch_cfg |= MASK_RESET_INT_LATCH;
  Serial.accelRegisterWrite(REG_LATCH_CFG_X21, latch_cfg);
  return value;
}

void BeanClass::setLedRed(uint8_t intensity) {
  LED_IND_SETTING_T setting;
  setting.color = (uint8_t)LED_RED;
  setting.intensity = intensity;

  Serial.ledSetSingle(setting);
}

void BeanClass::setLedGreen(uint8_t intensity) {
  LED_IND_SETTING_T setting;
  setting.color = (uint8_t)LED_GREEN;
  setting.intensity = intensity;

  Serial.ledSetSingle(setting);
}

void BeanClass::setLedBlue(uint8_t intensity) {
  LED_IND_SETTING_T setting;
  setting.color = (uint8_t)LED_BLUE;
  setting.intensity = intensity;

  Serial.ledSetSingle(setting);
}

void BeanClass::setLed(uint8_t red, uint8_t green, uint8_t blue) {
  LED_SETTING_T setting = {red, green, blue};
  Serial.ledSet(setting);
}

uint8_t BeanClass::getLedRed(void) {
  LED_SETTING_T reading;

  if (Serial.ledRead(&reading) == 0) {
    return reading.red;
  }

  return 0;
}

uint8_t BeanClass::getLedGreen(void) {
  LED_SETTING_T reading;

  if (Serial.ledRead(&reading) == 0) {
    return reading.green;
  }

  return 0;
}

uint8_t BeanClass::getLedBlue(void) {
  LED_SETTING_T reading;

  if (Serial.ledRead(&reading) == 0) {
    return reading.blue;
  }

  return 0;
}

LED_SETTING_T BeanClass::getLed(void) {
  LED_SETTING_T reading;

  if (Serial.ledRead(&reading) == 0) {
    return reading;
  }

  memset(&reading, 0, sizeof(reading));
  return reading;
}

ADV_SWITCH_ENABLED_T BeanClass::getServices(void) {
  ADV_SWITCH_ENABLED_T services;
  if (Serial.readGATT(&services) == 0) {
    return services;
  }

  memset(&services, 0, sizeof(ADV_SWITCH_ENABLED_T));
  return services;
}

void BeanClass::resetServices(void) {
  ADV_SWITCH_ENABLED_T services;
  memset(&services, 0, sizeof(ADV_SWITCH_ENABLED_T));
  services.standard = 1;
  setServices(services);
}

void BeanClass::setServices(ADV_SWITCH_ENABLED_T services) {
  Serial.writeGATT(services);
}

void BeanClass::setPairingPin(uint32_t pin) {
  if (pin <= 999999) {
    Serial.BTSetPairingPin(pin);
  }
}

void BeanClass::enablePairingPin(bool enable) {
  Serial.BTEnablePairingPin(enable);
}

void BeanClass::enableCustom(void) {
  ADV_SWITCH_ENABLED_T curServices = getServices();
  curServices.custom = 1;
  setServices(curServices);
}

void BeanClass::disableCustom(void) {
  ADV_SWITCH_ENABLED_T curServices = getServices();
  curServices.custom = 0;
  setServices(curServices);
}

void BeanClass::setCustomAdvertisement(uint8_t *buf, int len) {
  Serial.setCustomAdvertisement(buf, len);
}

int BeanClass::getObserverMessage(ObserverAdvertisementInfo *message,
                                  unsigned long timeout) {
  return Serial.getObserverMessage(message, timeout);
}

void BeanClass::enableiBeacon(void) {
  ADV_SWITCH_ENABLED_T curServices = getServices();
  curServices.ibeacon = 1;
  setServices(curServices);
}

bool BeanClass::setScratchData(uint8_t bank, const uint8_t *data,
                               uint8_t dataLength) {
  bool errorRtn = true;

  if (dataLength <= MAX_SCRATCH_SIZE) {
    BT_SCRATCH_T scratch;
    scratch.number = bank;
    memcpy((void *)scratch.scratch, (void *)data, dataLength);
    // magic: +1 due to bank byte
    Serial.BTSetScratchChar(&scratch, (uint8_t)(dataLength + 1));
  } else {
    errorRtn = false;
  }

  return errorRtn;
}

bool BeanClass::setScratchNumber(uint8_t bank, uint32_t data) {
  bool errorRtn = true;
  BT_SCRATCH_T scratch;
  scratch.number = bank;

  scratch.scratch[0] = data & 0xFF;
  scratch.scratch[1] = data >> 8UL;
  scratch.scratch[2] = data >> 16UL;
  scratch.scratch[3] = data >> 24UL;

  // magic: 4 for data, 1 for scratch bank
  Serial.BTSetScratchChar(&scratch, 4 + 1);

  return errorRtn;
}

ScratchData BeanClass::readScratchData(uint8_t bank) {
  ScratchData scratchTempBuffer;

  memset(scratchTempBuffer.data, 0, 20);
  Serial.BTGetScratchChar(bank, &scratchTempBuffer);

  return scratchTempBuffer;
}

long BeanClass::readScratchNumber(uint8_t bank) {
  long returnNum = 0;
  static ScratchData scratchNumBuffer;

  memset(scratchNumBuffer.data, 0, 20);
  Serial.BTGetScratchChar(bank, &scratchNumBuffer);

  returnNum |= (long)scratchNumBuffer.data[0] & 0xFF;
  returnNum |= (long)scratchNumBuffer.data[1] << 8UL;
  returnNum |= (long)scratchNumBuffer.data[2] << 16UL;
  returnNum |= (long)scratchNumBuffer.data[3] << 24UL;

  return returnNum;
}

void BeanClass::setBeanName(const String &name) {
  Serial.BTSetLocalName((const char *)name.c_str());
}

const char *BeanClass::getBeanName(void) {
  BT_RADIOCONFIG_T config;
  static char myChar[MAX_LOCAL_NAME_SIZE];

  int nameSize = MAX_LOCAL_NAME_SIZE;

  if (-1 != Serial.BTGetConfig(&config)) {
    nameSize = (config.local_name_size < MAX_LOCAL_NAME_SIZE)
                   ? config.local_name_size
                   : MAX_LOCAL_NAME_SIZE;

    memcpy((void *)myChar, (void *)config.local_name, nameSize);
  }

  // Null-terminate
  for (int i = nameSize; i < MAX_LOCAL_NAME_SIZE; i++) {
    myChar[i] = 0;
  }

  return myChar;
}

void BeanClass::setBeaconParameters(uint16_t uuid, uint16_t major_id,
                                    uint16_t minor_id) {
  Serial.BTSetBeaconParams(uuid, major_id, minor_id);
}

void BeanClass::setBeaconEnable(bool beaconEnable) {
  Serial.BTBeaconModeEnable(beaconEnable);
}

void BeanClass::enableWakeOnConnect(bool enable) {
  Serial.enableWakeOnConnect(enable);
}

void BeanClass::disconnect(void) { Serial.BTDisconnect(); }
