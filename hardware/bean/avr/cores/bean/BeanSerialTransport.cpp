#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <inttypes.h>
#include "wiring_private.h"

#include "BeanSerialTransport.h"

// There is a compiler or hardware bug(?) that causes
// HardwareSerial::write() to lock the Serial Port unless
// it is explicitely called with a uint8_t.
// I've copied the #defines into uint8_t types to ensure that
// HardwareSerial::write() isn't called with a #define by mistake.
const uint8_t BEAN_SOF = UT_CHAR_START;
const uint8_t BEAN_EOF = UT_CHAR_END;
const uint8_t BEAN_ESCAPE = UT_CHAR_ESC;
const uint8_t BEAN_ESCAPE_XOR = HDLC_ESCAPE_XOR;
static uint8_t m_ccSleepPinVal = LOW;

static const uint16_t BEAN_MIN_ADVERTISING_INT_MS = 20;    // ms
static const uint16_t BEAN_MAX_ADVERTISING_INT_MS = 1285;  // ms

#define MAX_BODY_LENGTH (APP_MSG_MAX_LENGTH - 2)

/*
 * on ATmega8, the uart and its bits are not numbered, so there is no "TXC0"
 * definition.
 */
#if !defined(TXC0)
#if defined(TXC)
#define TXC0 TXC
#elif defined(TXC1)
// Some devices have uart1 but no uart0
#define TXC0 TXC1
#else
#error TXC0 not definable in BeanSerialTransport.cpp
#endif
#endif

ring_buffer midi_buffer = {{0}, 0, 0};
ring_buffer ancs_buffer = {{0}, 0, 0};
ring_buffer ancs_message_buffer = {{0}, 0, 0};
ring_buffer observer_message = {{0}, 0, 0};
ring_buffer rx_buffer = {{0}, 0, 0};
ring_buffer tx_buffer = {{0}, 0, 0};
ring_buffer reply_buffer = {{0}, 0, 0};

static volatile bool tx_buffer_flushed = true;
static volatile bool serial_message_complete = false;

static volatile bool observer_message_sending = false;
static volatile int observer_msg_len = 0;

static inline void store_char(unsigned char c, ring_buffer *buffer) {
  unsigned int i = (buffer->head + 1) % SERIAL_BUFFER_SIZE;

  // if we should be storing the received character into the location
  // just before the tail (meaning that the head would advance to the
  // current location of the tail), we're about to overflow the buffer
  // and so we don't write the character or advance the head.
  if (i != buffer->tail) {
    buffer->buffer[buffer->head] = c;
    buffer->head = i;
  }
}

static uint32_t calc_crc32(uint32_t crc, uint8_t *buf, uint16_t len) {
  uint16_t k;

  crc = ~crc;
  while (len--) {
    crc ^= *buf++;
    for (k = 0; k < 8; k++) {
      // If CRC LSB is 1, right shift CRC, then XOR CRC with 0xEDB88320
      // Otherwise, just right shift CRC
      crc = crc & 1 ? (crc >> 1) ^ 0xedb88320 : crc >> 1;
    }
  }
  return ~crc;
}

void toUint8Array(uint32_t value, uint8_t *target, uint8_t target_bytes) {
  int i;
  uint8_t shift = target_bytes * 8;
  for (i = 0; i < target_bytes; i++) {
    shift -= 8;
    target[i] = (uint8_t)((value >> shift) & 0xFF);
  }
}

static bool rx_char(uint8_t *c) {
#if defined(UDR0)
  if (bit_is_clear(UCSR0A, UPE0)) {
    *c = UDR0;
    return true;
  } else {
    *c = UDR0;
    return false;
  }
#elif defined(UDR)
  if (bit_is_clear(UCSRA, PE)) {
    *c = UDR;
    return true;
  } else {
    *c = UDR;
    return false;
  }
#else
#error UDR not defined
#endif
}

#if !defined(USART0_RX_vect) && defined(USART1_RX_vect)
// do nothing - on the 32u4 the first USART is USART1
#else
#if !defined(USART_RX_vect) && !defined(USART0_RX_vect) && \
    !defined(USART_RXC_vect)
#error "Don't know what the Data Received vector is called for the first UART"
#else
void serialEvent() __attribute__((weak));
void serialEvent() {}
#define serialEvent_implemented
#if defined(USART_RX_vect)
ISR(USART_RX_vect)
#elif defined(USART0_RX_vect)
ISR(USART0_RX_vect)
#elif defined(USART_RXC_vect)
ISR(USART_RXC_vect)  // ATmega8
#endif
{
  // DECLARATIONS
  static enum {
    WAITING_FOR_SOF,
    GETTING_LENGTH,
    GETTING_MESSAGE_ID_1,
    GETTING_MESSAGE_ID_2,
    GETTING_MESSAGE_BODY,
    GETTING_CRC32,
    GETTING_EOF
  } bean_transport_state = WAITING_FOR_SOF;

  static bool escaping = false;
  static uint16_t messageType = MSG_ID_SERIAL_DATA;
  static uint8_t messageRemaining = 0;
  static uint8_t messageCur = 0;
  static uint8_t temp_var[4];
  static uint8_t rx_crc32[4];
  static uint32_t calculated_crc32;
  uint8_t bytes_ok;

  // buffer
  static ring_buffer *buffer = NULL;

  uint8_t next;
  if (!rx_char(&next)) {
    return;
  }

  // only handle escape char in message
  if (bean_transport_state != WAITING_FOR_SOF) {
    if (escaping == true) {
      next ^= BEAN_ESCAPE_XOR;
    } else if (escaping == false && next == BEAN_ESCAPE) {
      escaping = true;
      return;
    }
  }

  if (escaping == false) {
    if ((next == BEAN_SOF && bean_transport_state != WAITING_FOR_SOF) ||
        (next == BEAN_EOF && bean_transport_state != GETTING_EOF) ||
        next == BEAN_ESCAPE) {
      // RESET STATE
      escaping = false;
      messageType = WAITING_FOR_SOF;
      messageRemaining = 0;
      messageCur = 0;
      buffer = NULL;
      serial_message_complete = false;
      return;
    }
  }

  escaping = false;

  switch (bean_transport_state) {
    case WAITING_FOR_SOF:
      if (next == BEAN_SOF) {
        bean_transport_state = GETTING_LENGTH;
        serial_message_complete = false;
      }

      break;

    case GETTING_LENGTH:
      messageRemaining = next;
      bean_transport_state = GETTING_MESSAGE_ID_1;
      observer_msg_len = next;  // we don't have message type yet, but save the
                                // length for later
      calculated_crc32 = calc_crc32(0, &next, 1);
      break;

    case GETTING_MESSAGE_ID_1:
      messageType = ((unsigned int)next) << 8;
      messageRemaining--;
      bean_transport_state = GETTING_MESSAGE_ID_2;
      calculated_crc32 = calc_crc32(calculated_crc32, &next, 1);
      break;

    case GETTING_MESSAGE_ID_2:
      messageType |= next;
      messageRemaining--;

      if (messageType == MSG_ID_MIDI_READ) {
        buffer = &midi_buffer;
      } else if (messageType == MSG_ID_ANCS_READ) {
        buffer = &ancs_buffer;
      } else if (messageType == MSG_ID_ANCS_GET_NOTI) {
        buffer = &ancs_message_buffer;
      } else if (messageType == MSG_ID_OBSERVER_READ) {
        buffer = &observer_message;
        observer_message_sending = true;
        observer_message.head = observer_message.tail =
            0;  // if the user missed a previous message drop it
      } else {
        buffer =
            (messageType == MSG_ID_SERIAL_DATA) ? &rx_buffer : &reply_buffer;
      }

      if (messageRemaining > 0) {
        bean_transport_state = GETTING_MESSAGE_BODY;
      } else {
        bean_transport_state = GETTING_CRC32;
        messageRemaining = 4;
      }
      calculated_crc32 = calc_crc32(calculated_crc32, &next, 1);

      break;

    case GETTING_MESSAGE_BODY:
      if (buffer) {
        store_char(next, buffer);
        messageRemaining--;
        calculated_crc32 = calc_crc32(calculated_crc32, &next, 1);
      }

      if (messageRemaining == 0) {
        bean_transport_state = GETTING_CRC32;
        messageRemaining = 4;
      }
      break;
    case GETTING_CRC32:
      messageRemaining--;
      rx_crc32[3 - messageRemaining] = next;
      if (messageRemaining == 0) {
        toUint8Array(calculated_crc32, temp_var, 4);
        bean_transport_state = GETTING_EOF;
      }
      break;
    case GETTING_EOF:
      // RESET STATE
      if (messageType == MSG_ID_MIDI_READ) {
        for (int i = 0; i < 3; i++)
          // null message to specify the end of a BLE packet
          store_char(0, buffer);
      }
      if (messageType == MSG_ID_OBSERVER_READ) {
        observer_message_sending = false;
      }
      bytes_ok = 0;
      for (int i = 0; i < 4; i++) {
        if (temp_var[i] == rx_crc32[i]) {
          bytes_ok += 1;
        }
      }
      if (bytes_ok == 4) {
        serial_message_complete = true;
      }
      bean_transport_state = WAITING_FOR_SOF;
      messageType = MSG_ID_SERIAL_DATA;
      messageRemaining = 0;
      messageCur = 0;
      buffer = NULL;
      break;
  }
}
#endif
#endif

// The Original HWSerial version of this function
// relies on the TX Vector flag to tell when tx_buffer_flushed.
// we need that flag to fire the interrupt (auto-clears, and cannot be manually
// set) pin for the CC, so for BeanSerial we use 'tx_buffer_flushed' bool
// instead.
void BeanSerialTransport::flush() {
  // logic is handled in writes and interrupts
  while (tx_buffer_flushed == false) {}

  // this is a holdover from HWSerial.
  transmitting = false;
}

// This interrupt fires after the send has completed
ISR(USART_TX_vect) {
  // lower interrupt line that wakes The CC
  if (tx_buffer.head == tx_buffer.tail) {
    digitalWrite(CC_INTERRUPT_PIN, m_ccSleepPinVal);
    cbi(UCSR0B, TXCIE0);
    tx_buffer_flushed = true;
  }
}

// This interrupt fires after the send register as offloaded the data
// to the send hardware.
ISR(USART_UDRE_vect) {
  if (tx_buffer.head == tx_buffer.tail) {
    // Buffer empty, so disable interrupts
    cbi(UCSR0B, UDRIE0);

    // enable the tx sent interrupt so we can disable the
    // CC interrupt pin
    sbi(UCSR0B, TXCIE0);
  } else {
    // There is more data in the output buffer. Send the next byte
    unsigned char c = tx_buffer.buffer[tx_buffer.tail];
    tx_buffer.tail = (tx_buffer.tail + 1) % SERIAL_BUFFER_SIZE;

#if defined(UDR0)
    UDR0 = c;
#elif defined(UDR)
    UDR = c;
#else
#error UDR not defined
#endif
  }
}

// Called in main, before setup, to enable things such as setting the LED
// color during setup.
void BeanSerialTransport::begin(void) {
  HardwareSerial::begin(38400);
  pinMode(CC_INTERRUPT_PIN, OUTPUT);
  digitalWrite(CC_INTERRUPT_PIN, LOW);

  if (tx_buffer.head == tx_buffer.tail) {
    tx_buffer_flushed = true;
    digitalWrite(CC_INTERRUPT_PIN, m_ccSleepPinVal);
  }
}

inline void BeanSerialTransport::insert_escaped_char(uint8_t input) {
  // It's crucial that HardwareSerial::write is only called
  // with excplicit uint8_t types otherwise weird overloading
  // happens, and things you don't expect result
  switch (input) {
    case BEAN_SOF:  // fallthrough
    case BEAN_EOF:  // fallthrough
    case BEAN_ESCAPE:
      HardwareSerial::write(BEAN_ESCAPE);
      // without the cast, the XOR is getting promoted
      // to another type, probably an int, and causing unexpected
      // results in the messages.
      HardwareSerial::write(uint8_t(input ^ BEAN_ESCAPE_XOR));
      break;
    default:
      HardwareSerial::write(input);
  }
}

void BeanSerialTransport::BTConfigUartSleep(UART_SLEEP_MODE_T mode) {
  if (UART_SLEEP_NORMAL == mode) {
    m_wakeDelay = UART_DEFAULT_WAKE_WAIT;
    m_enforcedDelay = UART_DEFAULT_SEND_WAIT;
    m_ccSleepPinVal = LOW;
  } else if (UART_SLEEP_NEVER == mode) {
    m_wakeDelay = 0;
    m_enforcedDelay = 0;
    m_ccSleepPinVal = HIGH;
    digitalWrite(CC_INTERRUPT_PIN, HIGH);
  }
}

size_t BeanSerialTransport::write_message(uint16_t messageId,
                                          const uint8_t *body,
                                          size_t body_length) {
  static bool serial_initialized = false;
  uint32_t crc32 = 0;
  uint8_t temp_var[4];

  if (!serial_initialized) {
    Serial.begin();
    serial_initialized = true;
  }

  if (body_length > MAX_BODY_LENGTH) {
    return -1;
  }

  // if the buffer is empty, raise the ccinterrupt
  // and wait for the cc to wake before starting the transmit
  // testing has shown this to take up to 4ms.  adding 1 ms padding.
  tx_buffer_flushed = false;
  digitalWrite(CC_INTERRUPT_PIN, HIGH);
  if (tx_buffer.head == tx_buffer.tail && m_wakeDelay > 0) {
    delay(m_wakeDelay);
  }

  HardwareSerial::write(BEAN_SOF);
  // body_length + "2" for message type.
  temp_var[0] = body_length + 2;
  temp_var[1] = (uint8_t)(messageId >> 8);
  temp_var[2] = (uint8_t)(messageId & 0xFF);
  crc32 = calc_crc32(crc32, temp_var, 3);
  for (int i = 0; i < 3; i++) {
    insert_escaped_char(temp_var[i]);
  }

  crc32 = calc_crc32(crc32, (uint8_t *)body, body_length);
  for (uint8_t i = 0; i < body_length; i++) {
    insert_escaped_char(body[i]);
  }

  toUint8Array(crc32, temp_var, sizeof(uint32_t));
  for (uint8_t i = 0; i < sizeof(uint32_t); i++) {
    insert_escaped_char(temp_var[i]);
  }
  HardwareSerial::write(BEAN_EOF);
  // throttle the transfer speed
  if (m_enforcedDelay > 0) {
    delay(m_enforcedDelay);
  }

  return body_length;
}

int BeanSerialTransport::call_and_response(
    MSG_ID_T messageId, const uint8_t *body, size_t body_length,
    uint8_t *response, size_t *response_length, unsigned long timeout_ms) {
  noInterrupts();
  // clear our rx buffer to ensure that we don't read some old message out of it
  _reply_buffer->head = _reply_buffer->tail = 0;
  *_message_complete = false;
  interrupts();

  // send our message
  write_message(messageId, body, body_length);
  // wait for RX to hold an EOF, and then return the data

  _startMillis = millis();
  do {
  } while (*_message_complete == false &&
           (millis() - _startMillis < timeout_ms));

  if (*_message_complete) {
    // copy the message body into out
    memcpy(response, _reply_buffer->buffer,
           min(_reply_buffer->head, *response_length));
    *response_length = _reply_buffer->head;

    return 0;
  }

  return -1;
}

/////////
/// Radio
/////////

void BeanSerialTransport::BTSetAdvertisingOnOff(const bool setting,
                                                uint32_t timer) {
  BT_ADV_ONOFF_T advOnOff;
  advOnOff.adv_timer = timer;
  advOnOff.adv_onOff = setting ? 1 : 0;
  write_message(MSG_ID_BT_ADV_ONOFF, (const uint8_t *)&advOnOff,
                sizeof(advOnOff));
}

void BeanSerialTransport::BTSetLocalName(const char *name) {
  if (name == NULL) name = "";

  size_t length = strlen(name);
  if (length > 20) {
    length = 20;
  }

  BT_RADIOCONFIG_T radioConfig;
  size_t size = sizeof(BT_RADIOCONFIG_T);
  int response = call_and_response(MSG_ID_BT_GET_CONFIG, NULL, 0,
                                   (uint8_t *)&radioConfig, &size);

  if (0 == response) {
    memcpy((void *)radioConfig.local_name, (void *)name, length);
    radioConfig.local_name_size = length;

    uint16_t msgId =
        (m_enableSave ? MSG_ID_BT_SET_CONFIG : MSG_ID_BT_SET_CONFIG_NOSAVE);

    write_message(msgId, (const uint8_t *)&radioConfig, size);
  }
}

int BeanSerialTransport::BTGetStates(BT_STATES_T *btStates) {
  size_t length = sizeof(BT_STATES_T);
  return call_and_response(MSG_ID_BT_GET_STATES, NULL, (size_t)0,
                           (uint8_t *)btStates, &length);
}

void BeanSerialTransport::BTSetPairingPin(const uint32_t pin) {
  uint8_t msg[6] = {0};
  memcpy(msg, (void *)&pin, sizeof(pin));
  msg[4] = 0x01;    // 4th byte is the enable/disable for the pairing pin
  msg[5] = m_enableSave ? 1 : 0;  // 5th byte is for persistent memory
  write_message(MSG_ID_BT_SET_PIN, (const uint8_t *)msg, sizeof(msg));
}

void BeanSerialTransport::BTEnablePairingPin(bool enable) {
  uint8_t msg[2] = {0};
  msg[0] = enable ? 0x01 : 0x00;  // enable/disable
  msg[1] = m_enableSave ? 0x01 : 0x00;  // configsave
  write_message(MSG_ID_BT_ENABLE_PAIRING_PIN, (const uint8_t *)msg, sizeof(msg));
}

void BeanSerialTransport::BTSetAdvertisingInterval(uint16_t interval_ms) {
  if (interval_ms < BEAN_MIN_ADVERTISING_INT_MS) {
    interval_ms = BEAN_MIN_ADVERTISING_INT_MS;
  } else if (interval_ms > BEAN_MAX_ADVERTISING_INT_MS) {
    interval_ms = BEAN_MAX_ADVERTISING_INT_MS;
  }

  BT_RADIOCONFIG_T radioConfig;
  size_t size = sizeof(BT_RADIOCONFIG_T);
  int response = call_and_response(MSG_ID_BT_GET_CONFIG, NULL, 0,
                                   (uint8_t *)&radioConfig, &size);

  if (0 == response) {
    radioConfig.adv_int = (uint16_t)interval_ms;

    uint16_t msgId =
        (m_enableSave ? MSG_ID_BT_SET_CONFIG : MSG_ID_BT_SET_CONFIG_NOSAVE);

    write_message(msgId, (const uint8_t *)&radioConfig, size);
  }
}

void BeanSerialTransport::BTSetConnectionInterval(const int interval_ms) {
  write_message(MSG_ID_BT_SET_CONN, (const uint8_t *)&interval_ms, sizeof(int));
}

void BeanSerialTransport::BTSetTxPower(const BT_TXPOWER_DB_T &power) {
  write_message(MSG_ID_BT_SET_TX_PWR, (const uint8_t *)&power,
                sizeof(BT_TXPOWER_DB_T));
}

void BeanSerialTransport::BTSetScratchChar(BT_SCRATCH_T *setting,
                                           uint8_t length) {
  write_message(MSG_ID_BT_SET_SCRATCH, (uint8_t *)setting, (size_t)length);
}

int BeanSerialTransport::BTGetScratchChar(uint8_t scratchNum,
                                          ScratchData *scratchData) {
  int rtnVal;
  size_t lengthSt = sizeof(BT_SCRATCH_T);
  uint8_t buffer[1] = {scratchNum};

  rtnVal = call_and_response(MSG_ID_BT_GET_SCRATCH, buffer, (size_t)1,
                             (uint8_t *)scratchData, &lengthSt);
  // magic: -1 for length byte
  scratchData->length = (uint8_t)lengthSt - 1;

  return rtnVal;
}

int BeanSerialTransport::BTGetConfig(BT_RADIOCONFIG_T *config) {
  size_t size = sizeof(BT_RADIOCONFIG_T);
  return call_and_response(MSG_ID_BT_GET_CONFIG, NULL, 0, (uint8_t *)config,
                           &size);
}

void BeanSerialTransport::BTBeaconModeEnable(bool beaconEnable) {
  BT_RADIOCONFIG_T radioConfig;
  size_t size = sizeof(BT_RADIOCONFIG_T);
  int response = call_and_response(MSG_ID_BT_GET_CONFIG, NULL, 0,
                                   (uint8_t *)&radioConfig, &size);

  if (0 == response) {
    radioConfig.adv_mode = (beaconEnable ? ADV_IBEACON : ADV_STANDARD);
    uint16_t msgId =
        (m_enableSave ? MSG_ID_BT_SET_CONFIG : MSG_ID_BT_SET_CONFIG_NOSAVE);

    write_message(msgId, (const uint8_t *)&radioConfig, size);
  }
}

void BeanSerialTransport::BTSetBeaconParams(uint16_t uuid, uint16_t majorid,
                                            uint16_t minorid) {
  BT_RADIOCONFIG_T radioConfig;
  size_t size = sizeof(BT_RADIOCONFIG_T);
  int response = call_and_response(MSG_ID_BT_GET_CONFIG, NULL, 0,
                                   (uint8_t *)&radioConfig, &size);

  if (0 == response) {
    radioConfig.ibeacon_uuid = uuid;
    radioConfig.ibeacon_major = majorid;
    radioConfig.ibeacon_minor = minorid;

    uint16_t msgId =
        (m_enableSave ? MSG_ID_BT_SET_CONFIG : MSG_ID_BT_SET_CONFIG_NOSAVE);

    write_message(msgId, (const uint8_t *)&radioConfig, size);
  }
}

////////
// LED
////////
void BeanSerialTransport::ledSetSingle(const LED_IND_SETTING_T &setting) {
  write_message(MSG_ID_CC_LED_WRITE, (const uint8_t *)&setting,
                sizeof(setting));
}

void BeanSerialTransport::ledSet(const LED_SETTING_T &setting) {
  write_message(MSG_ID_CC_LED_WRITE_ALL, (const uint8_t *)&setting,
                sizeof(setting));
}

int BeanSerialTransport::ledRead(LED_SETTING_T *reading) {
  size_t size = sizeof(LED_SETTING_T);
  return call_and_response(MSG_ID_CC_LED_READ_ALL, NULL, 0, (uint8_t *)reading,
                           &size);
}

//////
/// GATT manager
//////

int BeanSerialTransport::readGATT(ADV_SWITCH_ENABLED_T *reading) {
  size_t size = sizeof(ADV_SWITCH_ENABLED_T);
  return call_and_response(MSG_ID_GATT_GET_GATT, NULL, 0, (uint8_t *)reading,
                           &size);
}

int BeanSerialTransport::writeGATT(ADV_SWITCH_ENABLED_T services) {
  write_message(MSG_ID_GATT_SET_GATT, (const uint8_t *)&services,
                sizeof(services));
}

int BeanSerialTransport::setCustomAdvertisement(uint8_t *buf, int len) {
  if (len > 31) {
    return -1;
  }
  uint8_t sendBuf[32];
  sendBuf[0] = len;
  memcpy((void *)&sendBuf[1], buf, len);
  write_message(MSG_ID_GATT_SET_CUSTOM, sendBuf, len + 1);
  return 0;
}

////////
/// MIDI
////////

char BeanSerialTransport::peekMidi() {
  return midi_buffer.buffer[midi_buffer.tail];
}
size_t BeanSerialTransport::midiAvailable() {
  if (midi_buffer.head == midi_buffer.tail) return 0;
  if (midi_buffer.head > midi_buffer.tail)
    return midi_buffer.head - midi_buffer.tail;
  else
    return midi_buffer.head + (SERIAL_BUFFER_SIZE - midi_buffer.tail);
}
size_t BeanSerialTransport::readMidi(uint8_t *buffer, size_t max_length) {
  size_t bytes_written = 0;
  while (midi_buffer.tail != midi_buffer.head) {
    if (bytes_written >= max_length) break;
    buffer[bytes_written] = midi_buffer.buffer[midi_buffer.tail];
    midi_buffer.tail++;
    midi_buffer.tail %= SERIAL_BUFFER_SIZE;
    bytes_written++;
  }
  return bytes_written;
}

void BeanSerialTransport::midiSend(uint8_t status, uint8_t byte1,
                                   uint8_t byte2) {
  uint8_t midi[3];
  midi[0] = status;
  midi[1] = byte1;
  midi[2] = byte2;
  write_message(MSG_ID_MIDI_WRITE, (const uint8_t *)midi, 3);
}

////////
// ANCS
////////

int BeanSerialTransport::ancsAvailable() {
  if (ancs_buffer.head == ancs_buffer.tail) return 0;
  if (ancs_buffer.head > ancs_buffer.tail)
    return (ancs_buffer.head - ancs_buffer.tail) / 8;
  else
    return (ancs_buffer.head + (SERIAL_BUFFER_SIZE - ancs_buffer.tail)) / 8;
}

int BeanSerialTransport::readAncs(uint8_t *buffer, size_t max_length) {
  size_t bytes_written = 0;
  while (ancs_buffer.tail != ancs_buffer.head) {
    if (bytes_written >= max_length) break;
    buffer[bytes_written] = ancs_buffer.buffer[ancs_buffer.tail];
    ancs_buffer.tail++;
    ancs_buffer.tail %= SERIAL_BUFFER_SIZE;
    bytes_written++;
  }
  return bytes_written;
}

int BeanSerialTransport::getAncsNotiDetails(uint8_t *buffer, size_t length,
                                                  uint8_t *data, uint32_t timeout) {
  ancs_message_buffer.head = ancs_message_buffer.tail;  //  clear buffer
  write_message(MSG_ID_ANCS_GET_NOTI, (const uint8_t *)buffer, length);
  uint32_t startMillis = millis();

  do {
    if ((millis() - startMillis > timeout)) return 0;
  } while (ancsNotiDetailsAvailable() < 8);  //  block until we have length bytes
  uint8_t buf[8];
  readAncsMessage(buf, 8);
  uint16_t incomingMsgLen = buf[6] | (buf[7] << 8);

  uint16_t bytesRead = 0;
  do {
    //  add bytes to buffer as they come in, this allows very large data arrays
    bytesRead += readAncsMessage((uint8_t *)&data[bytesRead], incomingMsgLen);
    if ((millis() - startMillis > timeout)) return bytesRead;
  } while (bytesRead < incomingMsgLen);

  return bytesRead;
}

int BeanSerialTransport::ancsNotiDetailsAvailable() {
  if (ancs_message_buffer.head == ancs_message_buffer.tail) return 0;
  if (ancs_message_buffer.head > ancs_message_buffer.tail)
    return (ancs_message_buffer.head - ancs_message_buffer.tail);
  else
    return (ancs_message_buffer.head +
            (SERIAL_BUFFER_SIZE - ancs_message_buffer.tail));
}

int BeanSerialTransport::readAncsMessage(uint8_t *buffer, size_t max_length) {
  size_t bytes_written = 0;
  while (ancs_message_buffer.tail != ancs_message_buffer.head) {
    if (bytes_written >= max_length) break;
    buffer[bytes_written] =
        ancs_message_buffer.buffer[ancs_message_buffer.tail];
    ancs_message_buffer.tail++;
    ancs_message_buffer.tail %= SERIAL_BUFFER_SIZE;
    bytes_written++;
  }
  return bytes_written;
}

///////
// Observer
///////
int BeanSerialTransport::getObserverMessage(OBSERVER_INFO_MESSAGE_T *message,
                                            unsigned long timeout) {
  // Begin observing
  write_message(MSG_ID_OBSERVER_START, NULL, 0);

  // Wait for Observed ADV if Any
  memset(message, 0, sizeof(OBSERVER_INFO_MESSAGE_T));

  unsigned long startMillis = millis();
  do {
    if ((millis() - startMillis > timeout)) {
      write_message(MSG_ID_OBSERVER_STOP, NULL, 0);
      return -1;
    }
  } while (observer_message_sending == false &&
           abs(observer_message.head - observer_message.tail) ==
               0);  // block until advertisement is observed
  do {
    if ((millis() - startMillis > timeout)) {
      write_message(MSG_ID_OBSERVER_STOP, NULL, 0);
      return -1;
    }
  } while (observer_message_sending == true);  // block until data is sent

  observer_message.head = observer_message.tail = 0;

  // copy the message body into out
  memcpy(message, observer_message.buffer,
         min(observer_msg_len, sizeof(OBSERVER_INFO_MESSAGE_T)));
  // Stop Observing
  write_message(MSG_ID_OBSERVER_STOP, NULL, 0);
  return 1;
}

////////
// Accelerometer
////////

int BeanSerialTransport::accelRead(ACC_READING_T *reading) {
  size_t size = sizeof(ACC_READING_T);
  return call_and_response(MSG_ID_CC_ACCEL_READ, NULL, (size_t)0,
                           (uint8_t *)reading, &size);
}

int BeanSerialTransport::accelRangeRead(uint8_t *range) {
  size_t size = sizeof(uint8_t);
  return call_and_response(MSG_ID_CC_ACCEL_GET_RANGE, NULL, (size_t)0,
                           (uint8_t *)range, &size);
}

void BeanSerialTransport::accelRangeSet(uint8_t range) {
  write_message(MSG_ID_CC_ACCEL_SET_RANGE, (const uint8_t *)&range,
                sizeof(range));
}

int BeanSerialTransport::accelRegisterRead(uint8_t reg, uint8_t length,
                                           uint8_t *value) {
  size_t size = sizeof(uint8_t);
  uint8_t payload[2];
  payload[0] = reg;
  payload[1] = length;
  return call_and_response(MSG_ID_CC_ACCEL_READ_REG, payload, sizeof(payload),
                           value, &size);
}

void BeanSerialTransport::accelRegisterWrite(uint8_t reg, uint8_t value) {
  uint8_t payload[2];
  payload[0] = reg;
  payload[1] = value;
  write_message(MSG_ID_CC_ACCEL_WRITE_REG, (const uint8_t *)&payload,
                sizeof(payload));
}

// Bit zero is INT1 pin from Accelerometer, Bit one is INT2 pin from
// Accelerometer (if available)
void BeanSerialTransport::wakeOnAccel(uint8_t int_enable) {
  uint8_t payload;
  payload = int_enable;
  write_message(MSG_ID_CC_WAKE_ON_ACCEL, (const uint8_t *)&payload,
                sizeof(payload));
}

/////////
// Temperature
/////////

int BeanSerialTransport::temperatureRead(int8_t *tempRead) {
  size_t size = 1;
  return call_and_response(MSG_ID_CC_TEMP_READ, NULL, (size_t)0,
                           (uint8_t *)tempRead, &size);
}

/////////
// Battery Level
/////////
int BeanSerialTransport::batteryRead(uint8_t *level) {
  size_t size = 1;
  return call_and_response(MSG_ID_CC_BATT_READ, NULL, (size_t)0,
                           (uint8_t *)level, &size);
}

////////
// Sleep
////////
void BeanSerialTransport::sleep(uint32_t duration_ms) {
  write_message(MSG_ID_AR_SLEEP, (const uint8_t *)&duration_ms,
                sizeof(duration_ms));
}

void BeanSerialTransport::enableWakeOnConnect(bool enable) {
  uint8_t enableBuff = (enable == true) ? 1 : 0;

  write_message(MSG_ID_AR_WAKE_ON_CONNECT, (const uint8_t *)&enableBuff,
                sizeof(enableBuff));
}

// Debug
bool BeanSerialTransport::debugLoopbackVerify(const uint8_t *message,
                                              const size_t size) {
  uint8_t res[size];  // NOLINT(runtime/arrays)
  size_t res_size = size;
  if (call_and_response(MSG_ID_DB_LOOPBACK, message, size, res, &res_size) !=
      0) {
    return false;
  }

  for (unsigned int i = 0; i < size; i++) {
    if (message[i] != res[i]) return false;
  }

  return true;
}

bool BeanSerialTransport::debugEndToEndLoopbackVerify(const uint8_t *message,
                                                      const size_t size) {
  uint8_t res[size];  // NOLINT(runtime/arrays)
  size_t res_size = size;
  // note that we've set the timeout to 250 here and not the default 100ms as
  // this is going to the phone and back.
  if (call_and_response(MSG_ID_DB_E2E_LOOPBACK, message, size, res, &res_size,
                        250) != 0) {
    return false;
  }

  for (unsigned int i = 0; i < size; i++) {
    if (message[i] != res[i]) return false;
  }

  return true;
}

int BeanSerialTransport::debugGetDebugCounter(int *counter) {
  size_t return_size;
  return call_and_response(MSG_ID_DB_COUNTER, NULL, 0, (uint8_t *)counter,
                           &return_size);
}

void BeanSerialTransport::debugWritePtm(const uint8_t *message,
                                        const size_t size) {
  write_message(MSG_ID_DB_PTM, message, size);
}

/////////////////////
/////////////////////
/////////////////////
// This is the public write function that is used all the time
size_t BeanSerialTransport::write(uint8_t c) {
  write_message(MSG_ID_SERIAL_DATA, &c, 1);
  return 1;
}

size_t BeanSerialTransport::write(const uint8_t *buffer, size_t size) {
  if (buffer == NULL || size == 0) return 0;

  if (size > MAX_BODY_LENGTH) {
    size_t end = MAX_BODY_LENGTH - 1;
    size_t start = 0;
    while (end <= size) {
      write_message(MSG_ID_SERIAL_DATA, buffer + start, end - start);
      if (end == size) break;
      start = end;
      end = min(end + MAX_BODY_LENGTH, size);
    }
    return size;
  } else {
    return write_message(MSG_ID_SERIAL_DATA, buffer, size);
  }
}

size_t BeanSerialTransport::print(const String &s) {
  return write((uint8_t *)s.c_str(), (size_t)s.length());
}

size_t BeanSerialTransport::print(const __FlashStringHelper *ifsh) {
  uint8_t buffer[MAX_BODY_LENGTH];
  const char PROGMEM *p = (const char PROGMEM *)ifsh;
  size_t n = 0;

  if (ifsh == NULL) return 0;

  while (1) {
    unsigned char c = pgm_read_byte(p++);
    if (c == 0) break;
    buffer[n++] = c;

    if (n == MAX_BODY_LENGTH) {
      write_message(MSG_ID_SERIAL_DATA, buffer, n);
      n = 0;
    }
  }
  write_message(MSG_ID_SERIAL_DATA, buffer, n);
  return n;
}

void BeanSerialTransport::debugLoopBackFullSerialMessages() {
  setTimeout(0);

  char buffer[APP_MSG_MAX_LENGTH + 1];

  while (1) {
    *_message_complete = false;

    // wait for RX to hold an EOF, and then return the data
    while (*_message_complete == false) {
      // BLOCK UNTIL WE GET THE ENTIRE RESPONSE
    }
    size_t length = APP_MSG_MAX_LENGTH + 1;
    length = readBytes(buffer, length);
    write((uint8_t *)buffer, length);
  }
}

void BeanSerialTransport::BTSetEnableConfigSave(bool enableSave) {
  m_enableSave = enableSave;
}

void BeanSerialTransport::BTDisconnect(void) {
  write_message(MSG_ID_BT_DISCONNECT, NULL, 0);
}

void BeanSerialTransport::BTRestart(void) {
  write_message(MSG_ID_BT_RESTART, NULL, 0);
}

// Preinstantiate Objects //////////////////////////////////////////////////////
#if defined(UBRRH) && defined(UBRRL)
BeanSerialTransport Serial(&rx_buffer, &tx_buffer, &UBRRH, &UBRRL, &UCSRA,
                           &UCSRB, &UCSRC, &UDR, RXEN, TXEN, RXCIE, UDRIE, U2X,
                           &reply_buffer, &serial_message_complete);
#elif defined(UBRR0H) && defined(UBRR0L)
BeanSerialTransport Serial(&rx_buffer, &tx_buffer, &UBRR0H, &UBRR0L, &UCSR0A,
                           &UCSR0B, &UCSR0C, &UDR0, RXEN0, TXEN0, RXCIE0,
                           UDRIE0, U2X0, &reply_buffer,
                           &serial_message_complete);
#elif defined(USBCON)
// do nothing - Serial object and buffers are initialized in CDC code
#else
#error no serial port defined, port 0
#endif
