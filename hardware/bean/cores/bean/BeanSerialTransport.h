
#ifndef BeanSerialTransport_h
#define BeanSerialTransport_h

#include "HardwareSerial.h"


class BeanSerialTransport : public HardwareSerial
{

protected:
  ring_buffer *_reply_buffer;
  void insert_escaped_char(uint8_t input);

  size_t write_message(uint16_t messageId, uint8_t* body, size_t body_length);

public:
  virtual size_t write(uint8_t);

// LED STUFF
    typedef struct {
      uint8_t red;
      uint8_t green;
      uint8_t blue;
      uint8_t intensity;
    } BeanLedSetting;

    void setLed(BeanLedSetting setting);
    BeanLedSetting readLed(void);


  // constructor
    BeanSerialTransport(ring_buffer *rx_buffer, ring_buffer *tx_buffer,
                        volatile uint8_t *ubrrh, volatile uint8_t *ubrrl,
                        volatile uint8_t *ucsra, volatile uint8_t *ucsrb,
                        volatile uint8_t *ucsrc, volatile uint8_t *udr,
                        uint8_t rxen, uint8_t txen, uint8_t rxcie,
                        uint8_t udrie, uint8_t u2x, ring_buffer* reply_buffer) 
                        : HardwareSerial(rx_buffer, tx_buffer, ubrrh, ubrrl,
                                         ucsra, ucsrb, ucsrc, udr, rxen, txen,
                                         rxcie, udrie, u2x)
  {
    _reply_buffer = reply_buffer;
  }; // End constructor

}; // End BeanSerialTransport


#if defined(UBRRH) || defined(UBRR0H)
  extern BeanSerialTransport Serial;
#elif defined(USBCON)
  #include "USBAPI.h"
//  extern HardwareSerial Serial_;  
#endif

#endif 