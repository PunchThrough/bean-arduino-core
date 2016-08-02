#ifndef BEAN_MIDI_H
#define BEAN_MIDI_H

#include "Arduino.h"

typedef enum {
    CHANNEL0 = 0x00,
    CHANNEL1 = 0x01,
    CHANNEL2 = 0x02,
    CHANNEL3 = 0x03,
    CHANNEL4 = 0x04,
    CHANNEL5 = 0x05,
    CHANNEL6 = 0x06,
    CHANNEL7 = 0x07,
    CHANNEL8 = 0x08,
    CHANNEL9 = 0x09,
    CHANNEL10 = 0x0A,
    CHANNEL11 = 0x0B,
    CHANNEL12 = 0x0C,
    CHANNEL13 = 0x0D,
    CHANNEL14 = 0x0E,
    CHANNEL15 = 0x0F
}midiChannels;

typedef enum {
    NOTE_Cn1    =   0,
    NOTE_Csn1   =   1,
    NOTE_Dn1    =   2,
    NOTE_Dsn1   =   3,
    NOTE_En1    =   4,
    NOTE_Fn1    =   5,
    NOTE_Fsn1   =   6,
    NOTE_Gn1    =   7,
    NOTE_Gsn1   =   8,
    NOTE_An1    =   9,
    NOTE_Asn1   =   10,
    NOTE_Bn1    =   11,
    NOTE_C0     =   12,
    NOTE_Cs0    =   13,
    NOTE_D0     =   14,
    NOTE_Ds0    =   15,
    NOTE_E0     =   16,
    NOTE_F0     =   17,
    NOTE_Fs0    =   18,
    NOTE_G0     =   19,
    NOTE_Gs0    =   20,
    NOTE_A0     =   21,
    NOTE_As0    =   22,
    NOTE_B0     =   23,
    NOTE_C1     =   24,
    NOTE_Cs1    =   25,
    NOTE_D1     =   26,
    NOTE_Ds1    =   27,
    NOTE_E1     =   28,
    NOTE_F1     =   29,
    NOTE_Fs1    =   30,
    NOTE_G1     =   31,
    NOTE_Gs1    =   32,
    NOTE_A1     =   33,
    NOTE_As1    =   34,
    NOTE_B1     =   35,
    NOTE_C2     =   36,
    NOTE_Cs2    =   37,
    NOTE_D2     =   38,
    NOTE_Ds2    =   39,
    NOTE_E2     =   40,
    NOTE_F2     =   41,
    NOTE_Fs2    =   42,
    NOTE_G2     =   43,
    NOTE_Gs2    =   44,
    NOTE_A2     =   45,
    NOTE_As2    =   46,
    NOTE_B2     =   47,
    NOTE_C3     =   48,
    NOTE_Cs3    =   49,
    NOTE_D3     =   50,
    NOTE_Ds3    =   51,
    NOTE_E3     =   52,
    NOTE_F3     =   53,
    NOTE_Fs3    =   54,
    NOTE_G3     =   55,
    NOTE_Gs3    =   56,
    NOTE_A3     =   57,
    NOTE_As3    =   58,
    NOTE_B3     =   59,
    NOTE_C4     =   60,
    NOTE_Cs4    =   61,
    NOTE_D4     =   62,
    NOTE_Ds4    =   63,
    NOTE_E4     =   64,
    NOTE_F4     =   65,
    NOTE_Fs4    =   66,
    NOTE_G4     =   67,
    NOTE_Gs4    =   68,
    NOTE_A4     =   69,
    NOTE_As4    =   70,
    NOTE_B4     =   71,
    NOTE_C5     =   72,
    NOTE_Cs5    =   73,
    NOTE_D5     =   74,
    NOTE_Ds5    =   75,
    NOTE_E5     =   76,
    NOTE_F5     =   77,
    NOTE_Fs5    =   78,
    NOTE_G5     =   79,
    NOTE_Gs5    =   80,
    NOTE_A5     =   81,
    NOTE_As5    =   82,
    NOTE_B5     =   83,
    NOTE_C6     =   84,
    NOTE_Cs6    =   85,
    NOTE_D6     =   86,
    NOTE_Ds6    =   87,
    NOTE_E6     =   88,
    NOTE_F6     =   89,
    NOTE_Fs6    =   90,
    NOTE_G6     =   91,
    NOTE_Gs6    =   92,
    NOTE_A6     =   93,
    NOTE_As6    =   94,
    NOTE_B6     =   95,
    NOTE_C7     =   96,
    NOTE_Cs7    =   97,
    NOTE_D7     =   98,
    NOTE_Ds7    =   99,
    NOTE_E7     =   100,
    NOTE_F7     =   101,
    NOTE_Fs7    =   102,
    NOTE_G7     =   103,
    NOTE_Gs7    =   104,
    NOTE_A7     =   105,
    NOTE_As7    =   106,
    NOTE_B7     =   107,
    NOTE_C8     =   108,
    NOTE_Cs8    =   109,
    NOTE_D8     =   110,
    NOTE_Ds8    =   111,
    NOTE_E8     =   112,
    NOTE_F8     =   113,
    NOTE_Fs8    =   114,
    NOTE_G8     =   115,
    NOTE_Gs8    =   116,
    NOTE_A8     =   117,
    NOTE_As8    =   118,
    NOTE_B8     =   119,
    NOTE_C9     =   120,
    NOTE_Cs9    =   121,
    NOTE_D9     =   122,
    NOTE_Ds9    =   123,
    NOTE_E9     =   124,
    NOTE_F9     =   125,
    NOTE_Fs9    =   126,
    NOTE_G9     =   127,
    NOTE_Gs9    =   128,
    NOTE_A9     =   129,
    NOTE_As9    =   130,
    NOTE_B9     =   131,
}midiNotes;

typedef enum {
    // High resolution Continuous Controllers MSB (+32 for LSB) ----------------
    BANKSELECT                  = 0,
    MODULATIONWHEEL             = 1,
    BREATHCONTROLLER            = 2,

    FOOTCONTROLLER              = 4,
    PORTAMENTOTIME              = 5,
    DATAENTRY                   = 6,
    CHANNELVOLUME               = 7,
    BALANCE                     = 8,

    PAN                         = 10,
    EXPRESSIONCONTROLLER        = 11,
    EFFECTCONTROL1              = 12,
    EFFECTCONTROL2              = 13,

    GENERALPURPOSECONTROLLER1   = 16,
    GENERALPURPOSECONTROLLER2   = 17,
    GENERALPURPOSECONTROLLER3   = 18,
    GENERALPURPOSECONTROLLER4   = 19,

    SUSTAIN                     = 64,
    PORTAMENTO                  = 65,
    SOSTENUTO                   = 66,
    SOFTPEDAL                   = 67,
    LEGATO                      = 68,
    HOLD                        = 69,

    SOUNDCONTROLLER1            = 70,
    SOUNDCONTROLLER2            = 71,
    SOUNDCONTROLLER3            = 72,
    SOUNDCONTROLLER4            = 73,
    SOUNDCONTROLLER5            = 74,
    SOUNDCONTROLLER6            = 75,
    SOUNDCONTROLLER7            = 76,
    SOUNDCONTROLLER8            = 77,
    SOUNDCONTROLLER9            = 78,
    SOUNDCONTROLLER10           = 79,
    GENERALPURPOSECONTROLLER5   = 80,
    GENERALPURPOSECONTROLLER6   = 81,
    GENERALPURPOSECONTROLLER7   = 82,
    GENERALPURPOSECONTROLLER8   = 83,
    PORTAMENTOCONTROL           = 84,

    EFFECTS1                    = 91,
    EFFECTS2                    = 92,
    EFFECTS3                    = 93,
    EFFECTS4                    = 94,
    EFFECTS5                    = 95,

    ALLSOUNDOFF                 = 120,
    RESETALLCONTROLLERS         = 121,
    LOCALCONTROL                = 122,
    ALLNOTESOFF                 = 123,
    OMNIMODEOFF                 = 124,
    OMNIMODEON                  = 125,
    MONOMODEON                  = 126,
    POLYMODEON                  = 127
}midiControlChange;

typedef enum {
    NOTEOFF             = 0x80,
    NOTEON              = 0x90,
    POLYPHONICPRESSURE  = 0xA0,
    CONTROLCHANGE       = 0xB0,
    PROGRAMCHANGE       = 0xC0,
    CHANNELPRESSURE     = 0xD0,
    PITCHBENDCHANGE     = 0xE0,
    SYSTEMCOMMON        = 0xF0,
    SYSTEMREALTIME      = 0xF8
}midiMessageTypes;

typedef enum  {
    BASS_DRUM_2     =   35,
    BASS_DRUM_1     =   36,
    SIDE_STICK      =   37,
    SNARE_DRUM_1    =   38,
    HAND_CLAP       =   39,
    SNARE_DRUM_2    =   40,
    LOW_TOM_2       =   41,
    CLOSED_HIHAT    =   42,
    LOW_TOM_1       =   43,
    PEDAL_HIHAT     =   44,
    MID_TOM_2       =   45,
    OPEN_HIHAT      =   46,
    MID_TOM_1       =   47,
    HIGH_TOM_2      =   48,
    CRASH_CYMBAL_1  =   49,
    HIGH_TOM_1      =   50,
    RIDE_CYMBAL_1   =   51,
    CHINESE_CYMBAL  =   52,
    RIDE_BELL       =   53,
    TAMBOURINE      =   54,
    SPLASH_CYMBAL   =   55,
    COWBELL         =   56,
    CRASH_CYMBAL_2  =   57,
    VIBRA_SLAP      =   58,
    RIDE_CYMBAL_2   =   59,
    HIGH_BONGO      =   60,
    LOW_BONGO       =   61,
    MUTE_HIGH_CONGA =   62,
    OPEN_HIGH_CONGA =   63,
    LOW_CONGA       =   64,
    HIGH_TIMBALE    =   65,
    LOW_TIMBALE     =   66,
    HIGH_AGOGO      =   67,
    LOW_AGOGO       =   68,
    CABASA          =   69,
    MARACAS         =   70,
    SHORT_WHISTLE   =   71,
    LONG_WHISTLE    =   72,
    SHORT_GUIRO     =   73,
    LONG_GUIRO      =   74,
    CLAVES          =   75,
    HIGH_WOOD_BLOCK =   76,
    LOW_WOOD_BLOCK  =   77,
    MUTE_CUICA      =   78,
    OPEN_CUICA      =   79,
    MUTE_TRIANGLE   =   80,
    OPEN_TRIANGLE   =   81
}midiDrums;


class BeanMidiClass {
 public:
  /****************************************************************************/
  /** @name MIDI
   *  Read and write MIDI packets via Bluetooth Low Energy.
   *  This profile uses Apple's <a href="https://developer.apple.com/library/ios/documentation/MusicAudio/Reference/CACoreMIDIRef/index.html">Core Midi Framework</a> to act as a musical instrument.  
   *  The Bean can send midi messages to an app like garage band to emulate diferent instruments.
   *  A list of different types of MIDI messages can be found <a href="https://www.midi.org/specifications/item/table-1-summary-of-midi-message">here</a>
   */
  ///@{

  /**
   *  Enables core Midi functionality on the Bean.
   *  
   *  # Examples
   *  When connected to iOS's or OSX's garage band this bean will play through the C Major scale, play a C Major chord, and then bend a note up.
   *  The Bean will also listen for incoming Core Midi messages.  If you have an app that can send core midi messages to BLE peripherals, the Bean's green LED will blink.
   *  
   *  @include profiles/MIDI.ino
   */
  void enable(void);

  /**
   *  @return true if Midi is currently enabled
   */
  bool isEnabled(void);

  /**
   *  Disables core Midi functionality on the Bean.
   */
  void disable(void);

  /**
   *  Loads and sends a buffer of Midi messages.
   *  @param buff a buffer of Midi messages.  Will only send Midi messages in groups of 3.  Must conform to the [Midi spec](https://www.midi.org/specifications/item/table-1-summary-of-midi-message).
   *  @param numBytes length of buffer
   *  @return number of Midi bytes sent, 0 if there are none to be sent
   */
  int sendMessage(uint8_t *buff, uint8_t numBytes);

  /**
   *  Loads and sends a single Midi message.
   *  @param status the status byte signifying the type of message
   *  @param byte1 the first data byte of the midi message
   *  @param byte2 the second data byte of the midi message
   *  @return number of Midi bytes sent, 0 if there are none to be sent
   */
  int sendMessage(uint8_t status, uint8_t byte1, uint8_t byte2);

  /**
   *  Reads a single incoming Midi message.
   *  @param pointer to the status the status byte signifying the type of message
   *  @param pointer to the byte1 the first data byte of the midi message
   *  @param pointer to the byte2 the second data byte of the midi message
   *  @return Midi status/timestamp byte
   */
  int readMessage(uint8_t *status, uint8_t *byte1, uint8_t *byte2);

  /**
   *  Sends Midi messages after they have been loaded to the midi buffer using loadMessage() commands.  
   *
   *  The buffer has a maximum size of 20 messages before it must be dumped or sent using this function.
   *
   *  @return number of Midi bytes sent, 0 if there are none to be sent
   */
  int sendMessages();

  /**
   *  Loads a message into the Midi buffer for sending using the sendMessages() function
   *  @param buff a buffer of Midi messages.  Will only send Midi messages in groups of 3.  Must conform to the [Midi spec](https://www.midi.org/specifications/item/table-1-summary-of-midi-message).
   *  @param numBytes length of buffer
   *  @return number of bytes successfully loaded
   */
  int loadMessage(uint8_t *buff, uint8_t numBytes);

  /**
   *  Loads a message into the Midi buffer for sending using the sendMessages() function
   *  @param status the status byte signifying the type of message
   *  @param byte1 the first data byte of the midi message
   *  @param byte2 the second data byte of the midi message
   *  @return 1 if bytes successfully loaded 0 if unsuccessful
   */
  int loadMessage(uint8_t status, uint8_t byte1, uint8_t byte2);

  /**
   *  Turns a specific note on for a specified midi channel.  The note will ring out until silenced via noteOff.
   *  @param channel takes a channel 0-15 or specified by midiChannels
   *  @param note takes a note from 0-127, can also take types midiNotes and midiDrums
   *  @param volume takes a volume values of 0-127
   */
  void noteOn(midiChannels channel, uint8_t note, uint8_t volume);

  /**
   *  Turns a specific note off for a specified midi channel.
   *  @param channel takes a channel 0-15 or specified by midiChannels
   *  @param note takes a note from 0-127, can also take types midiNotes and midiDrums
   *  @param volume takes a volume values of 0-127
   */
  void noteOff(midiChannels channel, uint8_t note, uint8_t volume);

  /**
   *  Bends the pitch of a note up or down
   *  @param channel takes a channel 0-15 or specified by midiChannels
   *  @value takes a pitch bend value from 0-16383 where 0x2000 is the middle or the true value of the note.
   */
  void pitchBend(midiChannels channel, uint16_t value);

  /**
   *  Enables or disables sustain for the midi channel
   *  @param channel takes a channel 0-15 or specified by midiChannels
   *  @param isOn set to true to enable sustain and false to disable
   */
  void sustain(midiChannels channel, bool isOn);

  ///@}
};



extern BeanMidiClass BeanMidi;

#endif
