// Holds the last data we read from scratch characteristic 1
ScratchData lastScratch;

// Compare two ScratchData objects.
// Return true if they contain the same bytes in the same order.
bool compareScratch(ScratchData *scratch1, ScratchData *scratch2) {
  // If they contain different numbers of bytes, they can't be equal,
  // so return false
  if (scratch1->length != scratch2->length) {
    return false;
  }

  // Compare each byte in order and return false if two bytes don't match
  for (int i = 0; i < scratch1->length; i++) {
    if (scratch1->data[i] != scratch2->data[i]) {
      return false;
    }
  }

  // If we've gotten this far, every byte in both ScratchData objects matches
  return true;
}

void setup() {
  // We don't need to set anything up here
}

void loop() {
  // Read data from scratch characteristic 1
  ScratchData thisScratch = Bean.readScratchData(1);

  // Compare the data we just read with the data we saw last time
  bool matched = compareScratch(&thisScratch, &lastScratch);

  // Save the data we just read for our next comparison
  lastScratch = thisScratch;

  if (!matched) {
    // Set the LED to green for 1 second if the data has changed
    Bean.setLed(0, 255, 0);
    Bean.sleep(1000);
    Bean.setLed(0, 0, 0);
  } else {
    // Otherwise, just sleep for one second and check again
    Bean.sleep(1000);
  }
}
