#include <Arduino.h>
#include "BluetoothMidi.h"
#include "Keyboard.h"

BluetoothMidi midi("Bomzhechort");
Keyboard keys;

void setup()
{
  Serial.begin(115200);
  keys.setup();
  midi.setup();
}

void loop()
{
  // If we've got a connection, we send an A4 during one second, at full velocity (127)
  if(midi.isConnected())
  {
      keys.play(midi);
  }
}
