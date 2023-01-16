#include <Arduino.h>

#include <Control_Surface.h>
#include "PCF8574.h"

BluetoothMIDI_Interface midi;

PCF8574 pcf[5] = {0x20, 0x21, 0x22, 0x23, 0x24};

// MIDI Note middle C (C in the fourth octave):
NoteButton button = {pcf[0].pin(0), {MIDI_Notes::note(MIDI_Notes::C, 4)}};

void setup()
{
  Serial.begin(115200);
  Control_Surface.begin();
}

void loop()
{
  Control_Surface.loop();
}
