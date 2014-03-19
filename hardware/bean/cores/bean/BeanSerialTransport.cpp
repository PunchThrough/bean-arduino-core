
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <inttypes.h>
#include "Arduino.h"
#include "wiring_private.h"


#include "BeanSerialTransport.h"


#define  MSG_SERIAL              0x0000
#define  MSG_BOOT_FWBLOCK        0x1000
#define  MSG_BOOT_STATUS_REQ     0x1001
#define  MSG_BOOT_STATUS         0x1002
#define  MSG_LED_WRITE           0x2000
#define  MSG_LED_READ            0x2001
#define  MSG_LED_READ_RESP       0x2001
#define  MSG_LED_WRITE_ALL       0x2081
#define  MSG_LED_READ_ALL        0x2003
#define  MSG_LED_READ_ALL_RESP   0x2004
#define  MSG_START_BLE_ADVERT    0x3000
#define  MSG_STOP_BLE_ADVERT     0x3001
#define  MSG_SET_BLE_INTERVAL    0x3002
#define  MSG_GET_BLE_INTERVAL    0x3003
#define  MSG_SET_TX_POWER        0x3004
#define  MSG_GET_TX_POWER        0x3005
#define  MSG_SET_ATMEL_INTERVAL  0x4000
#define  MSG_GET_ATMEL_INTERVAL  0x4001
#define  MSG_ATMEL_POWER_OFF     0x4002
#define  MSG_LOOPBACK_DEBUG_TX   0xFE00
#define  MSG_LOOPBACK_DEBUG_RX   0xFE80
#define  MSG_GET_DEBUG_COUNT_TX  0xFE01
#define  MSG_GET_DEBUG_COUNT_RX  0xFE81
#define  MSG_INVALID_TYPE        0xFFFF

const uint8_t  BEAN_SOF     = 0x7E;
const uint8_t  BEAN_EOF     = 0x7F;
const uint8_t  BEAN_ESCAPE  = 0x7D;

#define MAX_MESSAGE_LENGTH (SERIAL_BUFFER_SIZE - 2)
#define MAX_BODY_LENGTH (MAX_MESSAGE_LENGTH - 2)



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
#error TXC0 not definable in HardwareSerial.h
#endif
#endif




#if defined(USBCON)
  ring_buffer rx_buffer = { { 0 }, 0, 0};
  ring_buffer tx_buffer = { { 0 }, 0, 0};
  ring_buffer reply_buffer = { { 0 }, 0, 0};
#endif
#if defined(UBRRH) || defined(UBRR0H)
  ring_buffer rx_buffer  =  { { 0 }, 0, 0 };
  ring_buffer tx_buffer  =  { { 0 }, 0, 0 };
  ring_buffer reply_buffer = { { 0 }, 0, 0};
#endif

 static volatile bool serial_message_complete = false;

static inline void store_char(unsigned char c, ring_buffer *buffer){
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


static bool rx_char(uint8_t *c){
#if defined(UDR0)
  if (bit_is_clear(UCSR0A, UPE0)) {
    *c = UDR0;
    return true;
  } else {
    *c = UDR0;
    return false;
  };
#elif defined(UDR)
  if (bit_is_clear(UCSRA, PE)) {
    *c = UDR;
    return true;
  } else {
    *c = UDR;
    return false;
  };
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
  ISR(USART_RXC_vect) // ATmega8
#endif
  {

    // DECLARATIONS
    static enum {
      WAITING_FOR_SOF,
      GETTING_LENGTH,
      GETTING_MESSAGE_ID_1,
      GETTING_MESSAGE_ID_2,
      GETTING_MESSAGE_BODY,
      GETTING_EOF
    } bean_transport_state = WAITING_FOR_SOF;

    static bool escaping = false;
    static uint16_t messageType = MSG_INVALID_TYPE;
    static uint8_t messageRemaining = 0;
    static uint8_t messageCur = 0;

    // buffer
    static ring_buffer* buffer = NULL;

    uint8_t next;
    if(!rx_char(&next)){
      return;
    }
    

    // only handle escape char in message
    if(bean_transport_state != WAITING_FOR_SOF){
      if(escaping == true){
        next |= 0x20;
      }
      else if(escaping == false && next == BEAN_ESCAPE){
        escaping = true;
        return;
      }
    }

    if(escaping == false){
      if((next == BEAN_SOF && bean_transport_state != WAITING_FOR_SOF) ||
        ( next == BEAN_EOF && bean_transport_state != GETTING_EOF)     ||
          next == BEAN_ESCAPE){

        // TODO: How to Signal Error? 
        // assert(1);

        // RESET STATE
        escaping = false;
        messageType = MSG_INVALID_TYPE;
        messageRemaining = 0;
        messageCur = 0;
        buffer = NULL;

        return;
      }
    }

    escaping = false;

    switch(bean_transport_state){
      case WAITING_FOR_SOF:
        if(next == BEAN_SOF){
          bean_transport_state = GETTING_LENGTH;
          serial_message_complete = false;
        }
        break;

      case GETTING_LENGTH:
        messageRemaining = next;
        bean_transport_state = GETTING_MESSAGE_ID_1;
        break;


      case GETTING_MESSAGE_ID_1:
        messageType = ((unsigned int) next) << 8;
        messageRemaining--;
        bean_transport_state = GETTING_MESSAGE_ID_2;
        break;

      case GETTING_MESSAGE_ID_2:
        messageType |= next;
        messageRemaining--;

        buffer = (messageType == MSG_SERIAL) ? &rx_buffer : &reply_buffer;
        bean_transport_state = GETTING_MESSAGE_BODY;
        break;


      case GETTING_MESSAGE_BODY:
        if(buffer){
          store_char(next, buffer);
          messageRemaining--;
        }
        
        if(messageRemaining == 0){
          bean_transport_state = GETTING_EOF;
        }
      
      break;

      case GETTING_EOF:
        // TODO: How to Signal Error?
        // if(next != BEAN_EOF){
        //   assert(1);
        // }
        // RESET STATE
        serial_message_complete = true;

        messageType = MSG_INVALID_TYPE;
        messageRemaining = 0;
        messageCur = 0;
        buffer = NULL;
        break;
    }
  }
#endif
#endif


#if !defined(USART0_UDRE_vect) && defined(USART1_UDRE_vect)
// do nothing - on the 32u4 the first USART is USART1
#else
#if !defined(UART0_UDRE_vect) && !defined(UART_UDRE_vect) && !defined(USART0_UDRE_vect) && !defined(USART_UDRE_vect)
  #error "Don't know what the Data Register Empty vector is called for the first UART"
#else
#if defined(UART0_UDRE_vect)
ISR(UART0_UDRE_vect)
#elif defined(UART_UDRE_vect)
ISR(UART_UDRE_vect)
#elif defined(USART0_UDRE_vect)
ISR(USART0_UDRE_vect)
#elif defined(USART_UDRE_vect)
ISR(USART_UDRE_vect)
#endif
{
  if (tx_buffer.head == tx_buffer.tail) {
  // Buffer empty, so disable interrupts
#if defined(UCSR0B)
    cbi(UCSR0B, UDRIE0);
#else
    cbi(UCSRB, UDRIE);
#endif
  }
  else {
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
#endif
#endif


inline void BeanSerialTransport::insert_escaped_char(uint8_t input){
  switch(input){
    case BEAN_SOF:  // fallthrough
    case BEAN_EOF:  // fallthrough
    case BEAN_ESCAPE:
      HardwareSerial::write(BEAN_ESCAPE);
      HardwareSerial::write(input ^ 0x20);
    break;
    default:
      HardwareSerial::write(input);
  }
}

size_t BeanSerialTransport::write_message(uint16_t messageId,
                                          const uint8_t *body,
                                          size_t body_length){
  if(body_length > MAX_BODY_LENGTH){
    // TODO: do we want to throw an error, or somehow
    // note why we were forced to drop the message?
    return -1;
  }

  HardwareSerial::write(BEAN_SOF);
  // //body_length + "2" for message type.
  insert_escaped_char(body_length + 2);
  insert_escaped_char((uint8_t)(messageId >> 8));
  insert_escaped_char((uint8_t)(messageId & 0xFF));

  for(uint8_t i = 0; i < body_length; i++){
    insert_escaped_char(body[i]);
  }

  HardwareSerial::write(BEAN_EOF);

  // // TODO: Is this what we want to return?
   return body_length;
}


void BeanSerialTransport::call_and_response(uint16_t messageId, 
                                            const uint8_t *body,
                                           size_t body_length,
                                           uint8_t * response,
                                           size_t & response_length){
  
  noInterrupts();
  // clear our rx buffer to ensure that we don't read some old message out of it
  _reply_buffer->head = _reply_buffer->tail = 0;
  _message_complete = false;
  interrupts();

  // send our message
  write_message(messageId, body, body_length);

  //wait for RX to hold an EOF, and then return the data
  while(!_message_complete){
    // BLOCK UNTIL WE GET THE ENTIRE RESPONSE
    // TODO -- Timeout this?
  }

  // copy the message body into out
  memcpy(response, _reply_buffer->buffer,
         min(_reply_buffer->head, response_length));
  response_length = _reply_buffer->head;
} 

////////
// LED
////////
void BeanSerialTransport::setLed(BeanSerialTransport::BeanLedSetting &setting){
  write_message(MSG_LED_WRITE, (uint8_t*)&setting,
                sizeof(BeanSerialTransport::BeanLedSetting));
}

BeanSerialTransport::BeanLedSetting BeanSerialTransport::readLed(void){
  BeanSerialTransport::BeanLedSetting led;
  size_t response_size = sizeof(BeanSerialTransport::BeanLedSetting);
  call_and_response(MSG_LED_READ_ALL, (uint8_t*)NULL, (size_t)0, (uint8_t*)&led,
                    response_size);

  return led;
}

/////////
/// Radio
/////////
void BeanSerialTransport::setAdvertisingInterval(int interval_ms){
  write_message(MSG_SET_BLE_INTERVAL, (uint8_t*)&interval_ms, sizeof(int));
}

int BeanSerialTransport::advertisingInterval(void){
  int interval_ms = 0;
  size_t response_size = sizeof(int);
  call_and_response(MSG_GET_BLE_INTERVAL, (uint8_t*) NULL, (size_t)0,
                   (uint8_t*)&interval_ms, response_size);

  return interval_ms;
}

void BeanSerialTransport::setTxPower(BeanSerialTransport::TxPower_dB power){
  write_message(MSG_SET_TX_POWER, (uint8_t*)&power,
                sizeof(BeanSerialTransport::TxPower_dB));
}

BeanSerialTransport::TxPower_dB BeanSerialTransport::txPower(void){
  BeanSerialTransport::TxPower_dB power;
  size_t response_size = sizeof(BeanSerialTransport::TxPower_dB);
  call_and_response(MSG_GET_TX_POWER, (uint8_t*) NULL, (size_t)0,
                    (uint8_t*)&power, response_size);

  return power;
}


void BeanSerialTransport::setAtmegaPowerOnInterval(int interval_ms){
  write_message(MSG_SET_ATMEL_INTERVAL, (uint8_t*)&interval_ms, sizeof(int));
}

int BeanSerialTransport::atmegaPowerOnInterval(void){
  int interval_ms = 0;
  size_t response_size = sizeof(int);
  call_and_response(MSG_GET_ATMEL_INTERVAL, (uint8_t*) NULL, (size_t)0,
                   (uint8_t*)&interval_ms, response_size);

  return interval_ms;
}

void BeanSerialTransport::powerOff(void){
  write_message(MSG_ATMEL_POWER_OFF, (uint8_t*)NULL, 0);
}


/////////////////////
/////////////////////
/////////////////////
// This is the public write function that is used all the time
size_t BeanSerialTransport::write(uint8_t c)
{
  write_message(MSG_SERIAL, &c, 1);
  return  1;
}

size_t BeanSerialTransport::write(const uint8_t *buffer, size_t size)
{
  if(buffer == NULL)
    return 0;

  if(size > MAX_BODY_LENGTH){
    size_t end = MAX_BODY_LENGTH - 1;
    size_t start = 0;
    while(end <= size){
      write_message(MSG_SERIAL, buffer + start, end - start);
      if(end == size)
        break;
      start = end;
      end = min(end + MAX_BODY_LENGTH, size);
    }
    return size;
  }
  else
    return write_message(MSG_SERIAL, buffer, size);
}

size_t BeanSerialTransport::print(const String &s)
{
  return write((uint8_t*)s.c_str(), (size_t)s.length());
}

size_t BeanSerialTransport::print(const __FlashStringHelper *ifsh)
{
  uint8_t buffer[MAX_BODY_LENGTH];
  const char PROGMEM *p = (const char PROGMEM *)ifsh;
  size_t n = 0;

  if(ifsh == NULL)
    return 0;

  while (1) {
    unsigned char c = pgm_read_byte(p++);
    if (c == 0) break;
    buffer[n++] = c;

    if(n == MAX_BODY_LENGTH){
      write_message(MSG_SERIAL, buffer, n);
      n = 0;
    }
  }
  write_message(MSG_SERIAL, buffer, n);
  return n;
}





//Preinstantiate Objects //////////////////////////////////////////////////////
#if defined(UBRRH) && defined(UBRRL)
  BeanSerialTransport Serial(&rx_buffer, &tx_buffer, &UBRRH, &UBRRL, &UCSRA,
      &UCSRB, &UCSRC, &UDR, RXEN, TXEN, RXCIE, UDRIE, U2X, &reply_buffer,
      &serial_message_complete);
#elif defined(UBRR0H) && defined(UBRR0L)
  BeanSerialTransport Serial(&rx_buffer, &tx_buffer, &UBRR0H, &UBRR0L, &UCSR0A,
      &UCSR0B, &UCSR0C, &UDR0, RXEN0, TXEN0, RXCIE0, UDRIE0, U2X0, &reply_buffer,
      &serial_message_complete);
#elif defined(USBCON)
  // do nothing - Serial object and buffers are initialized in CDC code
#else
  #error no serial port defined  (port 0)
#endif
