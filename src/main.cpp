#include <Arduino.h>
#include "BluetoothMidi.h"

BluetoothMidi midi("Bomzhechort");

void setup()
{
  Serial.begin(115200);
  midi.setup();
}

void loop()
{
  // If we've got a connection, we send an A4 during one second, at full velocity (127)
  if(midi.isConnected())
  {
      midi.noteOn(0, 69, 127);
      delay(1000);
      midi.noteOff(0, 69, 127);        // Then we make a delay of one second before returning to the beginning of the loop
      delay(1000);
  }
}
