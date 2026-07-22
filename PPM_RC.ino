


void ppmISR() {
  uint32_t now = micros();
  uint32_t pulseWidth = now - lastEdgeTime;
  lastEdgeTime = now;

  if (pulseWidth >= SYNC_PULSE_MIN) {
    // End of frame: lock in channel count and restart
    numChannels  = channelIndex;
    channelIndex = 0;
    frameReady   = true;
  }
  else if (pulseWidth >= CHANNEL_MIN && pulseWidth <= CHANNEL_MAX) {
    if (channelIndex < MAX_CHANNELS) {
      ppmChannels[channelIndex++] = (uint16_t)pulseWidth;
      // Serial.print((uint16_t)pulseWidth);
      // Serial.print(" ");
    }
    // Serial.println("");

  }
  else {
    // Out-of-range pulse: discard the partial frame
    channelIndex = 0;
  }
}

void process_RC() {
  // Serial.println("here");
  
  if (frameReady) {
      // Serial.println("here too");

    frameReady = false;

    // Snapshot the volatile data with interrupts briefly disabled
    noInterrupts();
    uint8_t  n = numChannels;
    uint16_t values[MAX_CHANNELS];
    for (uint8_t i = 0; i < n; i++) values[i] = ppmChannels[i];
    interrupts();

    for (uint8_t i = 0; i < n; i++) {
      // Serial.print("CH");
      // Serial.print(i + 1);
      // Serial.print("=");
      RC_angle[i] = map(values[i], 1000, 2000, 0, 180);
      // Serial.print(RC_angle[i]);
      // RC_angle[i] = map(values[i], 1000, 2000, 0, 180);
      // Serial.print(i < n - 1 ? "  " : "\n");
    }
  }

  delay(20); // l


  // if (frameReady) {
  //   frameReady = false;

  //   // Snapshot the volatile data with interrupts briefly disabled
  //   noInterrupts();
  //   uint8_t  n = numChannels;
  //   int values[MAX_CHANNELS];
  //     Serial.print("i got here!");
  //   for (int i = 0; i < 10; i++) {
  //     Serial.print("i was here!");
  //     values[i] = ppmChannels[i];
  //     RC_angle[i] = map(values[i], 800, 2200, 0, 180);
  //     Serial.print( RC_angle[i]);
  //     Serial.print(" ");
  //     }
  //     Serial.println("");
  //   interrupts();
  // delay(20); // limit prints to ~50 Hz (one PPM frame period)

  //   }
  }

  




