#include "BluetoothMidi.h"

#include <BLEMidi.h>

void BluetoothMidi::setup()
{
  Serial.println("Initializing bluetooth");

  BLEMidiServer.begin(m_device_name);
  BLEMidiServer.setOnConnectCallback([](){
    Serial.println("Connected");
  });
  BLEMidiServer.setOnDisconnectCallback([](){
    Serial.println("Disconnected");
  });
  //BLEMidiServer.enableDebugging();  // Uncomment if you want to see some debugging output from the library (not much for the server class...)

  Serial.println("Waiting for connections...");
}

bool BluetoothMidi::isConnected() const
{
    return BLEMidiServer.isConnected();
}

void BluetoothMidi::noteOn(const uint8_t channel, const uint8_t note, const uint8_t velocity) const
{
    BLEMidiServer.noteOn(channel, note, velocity);
}

void BluetoothMidi::noteOff(const uint8_t channel, const uint8_t note, const uint8_t velocity) const
{
    BLEMidiServer.noteOff(channel, note, velocity);
}