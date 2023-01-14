#include <Arduino.h>
#include <BLEMidi.h>

void setup()
{
  Serial.begin(115200);
  Serial.println("Initializing bluetooth");
  BLEMidiServer.begin("Basic MIDI device");
  Serial.println("Waiting for connections...");
  //BLEMidiServer.enableDebugging();  // Uncomment if you want to see some debugging output from the library (not much for the server class...)
}

void loop()
{
  // If we've got a connection, we send an A4 during one second, at full velocity (127)
  if(BLEMidiServer.isConnected())
  {
      BLEMidiServer.noteOn(0, 69, 64);
      delay(1000);
      BLEMidiServer.noteOff(0, 69, 64);        // Then we make a delay of one second before returning to the beginning of the loop
      delay(1000);
  }
}
