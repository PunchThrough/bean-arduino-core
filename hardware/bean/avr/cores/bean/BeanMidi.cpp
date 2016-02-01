#include "BeanMidi.h"
#include "Bean.h"
#include "BeanSerialTransport.h"
#include "Arduino.h"


BeanMidiClass BeanMidi;

// midi access definitions
#define MIDI_BUFFER_SIZE 20
#define BLE_PACKET_SIZE 20

typedef struct {
  uint32_t timestamp;
  uint8_t status;
  uint8_t byte1;
  uint8_t byte2;
} midiMessage;

static midiMessage midiMessages[MIDI_BUFFER_SIZE];
uint8_t midiPacket[BLE_PACKET_SIZE];
uint8_t midiWriteOffset = 0;
uint8_t midiReadOffset = 0;

uint8_t lastStatus = 0;
long midiTimeStampDiff = 0;
bool midiPacketBegin = true;


void BeanMidiClass::enable(void) {
  ADV_SWITCH_ENABLED_T curServices = Bean.getServices();
  curServices.midi = 1;
  Bean.setServices(curServices);
}

bool BeanMidiClass::isEnabled(void) {
  ADV_SWITCH_ENABLED_T curServices = Bean.getServices();
  return (curServices.midi == 1);
}

void BeanMidiClass::disable(void) {
  ADV_SWITCH_ENABLED_T curServices = Bean.getServices();
  curServices.midi = 0;
  Bean.setServices(curServices);
}

int BeanMidiClass::loadMessage(uint8_t status, uint8_t byte1, uint8_t byte2) {
  if ((midiWriteOffset + 1) % MIDI_BUFFER_SIZE == midiReadOffset) return 0;
  uint32_t millisec = millis();
  midiMessages[midiWriteOffset].status = status;
  midiMessages[midiWriteOffset].byte1 = byte1;
  midiMessages[midiWriteOffset].byte2 = byte2;
  midiMessages[midiWriteOffset].timestamp = millisec;
  midiWriteOffset++;
  midiWriteOffset = midiWriteOffset % MIDI_BUFFER_SIZE;
  return 1;
}

int BeanMidiClass::sendMessages() {
  if (midiReadOffset == midiWriteOffset) return 0;
  uint8_t byteOffset = 0;
  // send a 20 byte message
  uint32_t millisec = midiMessages[midiReadOffset].timestamp;
  // first the header
  uint8_t head_ts = millisec >> 7;
  head_ts |= 1 << 7;  // set the 7th bit to 1
  head_ts &= ~(1 << 6);  // set the 6th bit to zero
  midiPacket[byteOffset++] = head_ts;
  // now some messages
  int lastStatus = -1;
  int lastTime = -1;
  while (midiReadOffset != midiWriteOffset) {
    if (lastStatus == midiMessages[midiReadOffset].status &&
        lastTime == midiMessages[midiReadOffset].timestamp) {
      midiPacket[byteOffset++] = midiMessages[midiReadOffset].byte1;
      midiPacket[byteOffset++] = midiMessages[midiReadOffset].byte2;
    } else {
      uint8_t msg_ts = midiMessages[midiReadOffset].timestamp;
      msg_ts |= 1 << 7;  // set the 7th bit to 1.
      midiPacket[byteOffset++] = msg_ts;
      midiPacket[byteOffset++] = midiMessages[midiReadOffset].status;
      midiPacket[byteOffset++] = midiMessages[midiReadOffset].byte1;
      midiPacket[byteOffset++] = midiMessages[midiReadOffset].byte2;
    }
    midiReadOffset++;
    midiReadOffset = midiReadOffset % MIDI_BUFFER_SIZE;
    if (byteOffset + 4 >
        BLE_PACKET_SIZE)  // can we handle another midi message in this packet
      break;
  }
  Serial.write_message(MSG_ID_MIDI_WRITE, midiPacket, byteOffset);
  return byteOffset;
}

int BeanMidiClass::readMessage(uint8_t *status, uint8_t *byte1, uint8_t *byte2) {
  uint8_t buffer[8];
  if (midiPacketBegin) {
    if (Serial.midiAvailable() > 4) {
      Serial.readMidi(buffer, 1);  // header
      midiPacketBegin = false;  // we are now in the body
    }
  }
  if (!midiPacketBegin) {
    // read the first byte, check if its a status byte
    if (Serial.midiAvailable() > 0) {
      uint8_t peek = 0;
      peek = Serial.peekMidi();
      if (peek & 1 << 7) {
        // is status/timestamp byte. we are looking at a 4 byte message
        if (Serial.midiAvailable() >= 4) {
          Serial.readMidi(buffer, 4);
          uint8_t timestamp = buffer[0];
          *status = buffer[1];
          lastStatus = *status;
          *byte1 = buffer[2];
          *byte2 = buffer[3];
          if (timestamp == 0xFF &&
              *status == 0xFF &&
              *byte1 == 0xFF &&
              *byte2 == 0xFF) {
            // end of packet
            midiPacketBegin = true;
            return 0;
          } else {
            return peek;
          }
        }
      } else {  // running status
        if (Serial.midiAvailable() >= 2) {
          Serial.readMidi(buffer, 2);
          *status = lastStatus;
          *byte1 = buffer[0];
          *byte2 = buffer[1];
          return peek;
        }
      }
    }
  }
  return 0;
}


/**
 *  Needs docs
 */
int BeanMidiClass::sendMessage(uint8_t *buff, uint8_t numBytes) {
  int idx = 0;
  while (numBytes >= 3) {
    if ((midiWriteOffset + 1) % MIDI_BUFFER_SIZE == midiReadOffset) sendMessages();
    loadMessage(buff[idx], buff[idx + 1], buff[idx + 2]);
    idx += 3;
    numBytes -= 3;
  }
  if (midiReadOffset != midiWriteOffset) {  // if we still have unsent messages
    return sendMessages();
  }
  return 0;
}

/**
 *  Needs docs
 */
int BeanMidiClass::sendMessage(uint8_t status, uint8_t byte1, uint8_t byte2) {
  loadMessage(status, byte1, byte2);
  sendMessages();
}

/**
 *  Needs docs
 */
int BeanMidiClass::loadMessage(uint8_t *buff, uint8_t numBytes) {
  int idx = 0;
  while (numBytes >= 3) {
    int isFull = loadMessage(buff[idx], buff[idx + 1], buff[idx + 2]);
    idx += 3;
    if (isFull == 0) return idx;
    numBytes -= 3;
  }
  return idx;
}

/**
 *  Needs docs
 */
void BeanMidiClass::noteOn(midiChannels channel, uint8_t note, uint8_t volume) {
  sendMessage(channel | NOTEON, (note & 0x7F), (volume & 0x7F));  // highest bit must be 0
}

/**
 *  Needs docs
 */
void BeanMidiClass::noteOff(midiChannels channel, uint8_t note, uint8_t volume) {
  sendMessage(channel | NOTEOFF, (note & 0x7F), (volume & 0x7F));  // highest bit must be 0
}

/**
 *  Needs docs
 */
void BeanMidiClass::pitchBend(midiChannels channel, uint16_t value) {
  if (value > 0x3FFF) {  //  pitch bend has a max of 14 bits
    value = 0x3FFF;
  }
  uint8_t lsb, msb = 0;
  lsb = value & 0x7F;         //  the highest bit must be 0
  msb = (value >> 7) & 0x7F;  //  the highest bit must be 0
  sendMessage(channel | PITCHBENDCHANGE, lsb, msb);
}

/**
 *  Needs docs
 */
void BeanMidiClass::sustain(midiChannels channel, bool isOn) {
  sendMessage(channel | CONTROLCHANGE, SUSTAIN, isOn ? 64 : 0);  //  ≤63 off, ≥64 on for sustain
}

