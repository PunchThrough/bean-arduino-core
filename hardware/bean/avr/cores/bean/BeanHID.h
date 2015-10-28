

#ifndef __BEANHID__
#define __BEANHID__

//==============================================================================
//==============================================================================
// Mouse

typedef enum
{
  MOUSE_LEFT = 1,
  MOUSE_RIGHT = 2,
  MOUSE_MIDDLE = 4,
  MOUSE_ALL = (MOUSE_LEFT | MOUSE_RIGHT | MOUSE_MIDDLE)
} mouseButtons;


#define HID_CC_IN_RPT_LEN 2

typedef struct { uint8_t mouse[5]; } BeanMouseReport;

class BeanMouse_ {
 private:
  uint8_t _buttons;
  void buttons(uint8_t b);
  void sendReport(BeanMouseReport* commands);

 public:
  BeanMouse_(void);
  void begin(void);
  void end(void);
  void click(uint8_t b = MOUSE_LEFT);
  void move(signed char x, signed char y, signed char wheel = 0);
  void press(uint8_t b = MOUSE_LEFT);      // press LEFT by default
  void release(uint8_t b = MOUSE_LEFT);    // release LEFT by default
  bool isPressed(uint8_t b = MOUSE_LEFT);  // check LEFT by default
};
extern BeanMouse_ BeanMouse;

//==============================================================================
//==============================================================================
// Keyboard
typedef enum 
{
  KEY_LEFT_CTRL=0x80,
  KEY_LEFT_SHIFT=0x81,
  KEY_LEFT_ALT=0x82,
  KEY_LEFT_GUI=0x83,
  KEY_RIGHT_CTRL=0x84,
  KEY_RIGHT_SHIFT=0x85,
  KEY_RIGHT_ALT=0x86,
  KEY_RIGHT_GUI=0x87,

  KEY_UP_ARROW=0xDA,
  KEY_DOWN_ARROW=0xD9,
  KEY_LEFT_ARROW=0xD8,
  KEY_RIGHT_ARROW=0xD7,
  KEY_BACKSPACE=0xB2,
  KEY_TAB=0xB3,
  KEY_RETURN=0xB0,
  KEY_ESC=0xB1,
  KEY_INSERT=0xD1,
  KEY_DELETE=0xD4,
  KEY_PAGE_UP=0xD3,
  KEY_PAGE_DOWN=0xD6,
  KEY_HOME=0xD2,
  KEY_END=0xD5,
  KEY_CAPS_LOC=0xC1,
  KEY_F1=0xC2,
  KEY_F2=0xC3,
  KEY_F3=0xC4,
  KEY_F4=0xC5,
  KEY_F5=0xC6,
  KEY_F6=0xC7,
  KEY_F7=0xC8,
  KEY_F8=0xC9,
  KEY_F9=0xCA,
  KEY_F10=0xCB,
  KEY_F11=0xCC,
  KEY_F12=0xCD
} modifierKey;

// HID Consumer Usage IDs (subset of the codes available in the USB HID Usage
// Tables spec)
typedef enum {
   HID_CONSUMER_POWER = 48,  // 0x30 - Power THIS
   HID_CONSUMER_ASSIGN_SEL = 129,    // 0x81 - Assign Selection THIS
   HID_CONSUMER_CHANNEL_UP = 156,    // 0x9C - Channel Increment THIS
   HID_CONSUMER_CHANNEL_DOWN = 157,  // 0x9D - Channel Decrement THIS
   HID_CONSUMER_RECALL_LAST = 131,   // 0x83 - Recall Last THIS
   HID_CONSUMER_PLAY = 176,           // 0xB0 - Play THIS
   HID_CONSUMER_PAUSE = 177,          // 0xB1 - Pause THIS
   HID_CONSUMER_RECORD = 178,         // 0xB2 - Record THIS
   HID_CONSUMER_FAST_FORWARD = 179,   // 0xB3 - Fast Forward THIS
   HID_CONSUMER_REWIND = 180,         // 0xB4 - Rewind THIS
   HID_CONSUMER_SCAN_NEXT_TRK = 181,  // 0xB5 - Scan Next Track THIS
   HID_CONSUMER_SCAN_PREV_TRK = 182,  // 0xB6 - Scan Previous Track THIS
   HID_CONSUMER_STOP = 183,           // 0xB7 - Stop THIS
   HID_CONSUMER_VOLUME_UP = 233,    // 0xE9 - Volume Increment THIS
   HID_CONSUMER_VOLUME_DOWN = 234,  // 0xEA - Volume Decrement THIS
   HID_CONSUMER_MUTE = 226         // 0xE2 - Mute THIS
} mediaControl;


#define HID_CONSUMER_RESET 49  // 0x31 - Reset
#define HID_CONSUMER_SLEEP 50  // 0x32 - Sleep

#define HID_CONSUMER_MENU 64           // 0x40 - Menu
#define HID_CONSUMER_SELECTION 128     // 0x80 - Selection
#define HID_CONSUMER_MODE_STEP 130     // 0x82 - Mode Step
#define HID_CONSUMER_QUIT 148          // 0x94 - Quit
#define HID_CONSUMER_HELP 149          // 0x95 - Help

#define HID_CONSUMER_EJECT 184          // 0xB8 - Eject
#define HID_CONSUMER_RANDOM_PLAY 185    // 0xB9 - Random Play
#define HID_CONSUMER_SELECT_DISC 186    // 0xBA - Select Disk
#define HID_CONSUMER_ENTER_DISC 187     // 0xBB - Enter Disc
#define HID_CONSUMER_REPEAT 188         // 0xBC - Repeat
#define HID_CONSUMER_STOP_EJECT 204     // 0xCC - Stop/Eject
#define HID_CONSUMER_PLAY_PAUSE 205     // 0xCD - Play/Pause
#define HID_CONSUMER_PLAY_SKIP 206      // 0xCE - Play/Skip

#define HID_CONSUMER_VOLUME 224       // 0xE0 - Volume
#define HID_CONSUMER_BALANCE 225      // 0xE1 - Balance
#define HID_CONSUMER_BASS 227         // 0xE3 - Bass


// HID Consumer Control keycodes (based on the HID Report Map characteristic
// value)
#define HID_CC_RPT_MUTE 1
#define HID_CC_RPT_POWER 2
#define HID_CC_RPT_LAST 3
#define HID_CC_RPT_ASSIGN_SEL 4
#define HID_CC_RPT_PLAY 5
#define HID_CC_RPT_PAUSE 6
#define HID_CC_RPT_RECORD 7
#define HID_CC_RPT_FAST_FWD 8
#define HID_CC_RPT_REWIND 9
#define HID_CC_RPT_SCAN_NEXT_TRK 10
#define HID_CC_RPT_SCAN_PREV_TRK 11
#define HID_CC_RPT_STOP 12

#define HID_CC_RPT_CHANNEL_UP 0x01
#define HID_CC_RPT_CHANNEL_DOWN 0x03
#define HID_CC_RPT_VOLUME_UP 0x40
#define HID_CC_RPT_VOLUME_DOWN 0x80

// HID Consumer Control report bitmasks
#define HID_CC_RPT_NUMERIC_BITS 0xF0
#define HID_CC_RPT_CHANNEL_BITS 0xCF
#define HID_CC_RPT_VOLUME_BITS 0x3F
#define HID_CC_RPT_BUTTON_BITS 0xF0
#define HID_CC_RPT_SELECTION_BITS 0xCF

#define HID_KEYBOARD_1 30  // 0x1E - Keyboard 1 and !
#define HID_KEYBOARD_0 39  // 0x27 - Keyboard 0 and )

// Low level key report: up to 6 keys and shift, ctrl etc at once
typedef struct {
  uint8_t modifiers;
  uint8_t reserved;
  uint8_t keys[6];
} BeanKeyReport;

class BeanKeyboard_ : public Print {
 private:
  BeanKeyReport _keyReport;
  uint8_t ccHoldBuffer[HID_CC_IN_RPT_LEN] = {0, 0};
  void sendReport(BeanKeyReport* keys);

 public:
  BeanKeyboard_(void);
  void begin(void);
  void end(void);
  void sendCC(uint8_t command);
  void holdCC(uint8_t command);
  void releaseCC(uint8_t command);

  virtual size_t write(uint8_t k);
  virtual size_t press(uint8_t k);
  virtual size_t release(uint8_t k);
  virtual void releaseAll(void);
};
extern BeanKeyboard_ BeanKeyboard;

//==============================================================================
//==============================================================================
// Low level API

typedef struct {
  uint8_t bmRequestType;
  uint8_t bRequest;
  uint8_t wValueL;
  uint8_t wValueH;
  uint16_t wIndex;
  uint16_t wLength;
} Setup;

#endif /* if defined(BeanHID) */
