void setup() {
  // We don't have to do anything here
}

// C Major Scale
char scale[8] = {NOTE_C4, NOTE_D4, NOTE_E4, NOTE_F4,
                 NOTE_G4, NOTE_A4, NOTE_B4, NOTE_C5};

void loop() {
  // play the C Major scale
  for (int i = 0; i < 8; i++) {
    BeanMidi.noteOn(CHANNEL1, scale[i], 60);
    delay(1000);
    BeanMidi.noteOff(CHANNEL1, scale[i], 60);
    checkMidi();
  }

  // C Major chord
  BeanMidi.noteOn(CHANNEL1, NOTE_C4, 60);
  BeanMidi.noteOn(CHANNEL1, NOTE_E4, 60);
  BeanMidi.noteOn(CHANNEL1, NOTE_G4, 60);
  delay(2000);
  BeanMidi.noteOff(CHANNEL1, NOTE_C4, 60);
  BeanMidi.noteOff(CHANNEL1, NOTE_E4, 60);
  BeanMidi.noteOff(CHANNEL1, NOTE_G4, 60);

  // pitch bend
  BeanMidi.noteOn(CHANNEL1, 60, 70);
  delay(1000);
  BeanMidi.pitchBend(CHANNEL1, 0x2500);
  delay(1000);
  BeanMidi.pitchBend(CHANNEL1, 0x2000);
  delay(1000);
  BeanMidi.noteOff(CHANNEL1, 60, 0);

  checkMidi();
}

// Poll MIDI to check if new messages have arrived
void checkMidi() {
  uint8_t stat, byte1, byte2;
  if (BeanMidi.readMessage(&stat, &byte1, &byte2)) {
    Bean.setLedGreen(130);
    delay(50);
    Bean.setLedGreen(0);
  }
}
