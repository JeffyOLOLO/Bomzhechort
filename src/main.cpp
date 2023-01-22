#include <Arduino.h>
#include <Wire.h>

#include <Control_Surface.h>
#include "PCF8574.h"
#include "MPR121.h"

// Didn't work with 12 and 13 pins, so do not use them for I²C
constexpr ArduinoPin_t I2C_SCL = 14;
constexpr ArduinoPin_t I2C_SDA = 15;

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wdeprecated-declarations"
using Octave_t = AH::function_traits<decltype(MIDI_Notes::note)>::argument_t<1>;
constexpr Octave_t OCTAVE = 4;
#pragma GCC diagnostic pop

constexpr auto CHORDS_CHANNEL  = CHANNEL_1;
constexpr auto STRINGS_CHANNEL = CHANNEL_2;

BluetoothMIDI_Interface midi;
// USBDebugMIDI_Interface midi = 115200;
// BidirectionalMIDI_Pipe mpipe;

constexpr auto PCF_INT_PIN = 13;
PCF8574 pcf = {0x20, PCF_INT_PIN};

NoteChordButton chords[3] = {
  { pcf.pin(0), {MIDI_Notes::Db(OCTAVE), CHORDS_CHANNEL}, Chords::Major },
  { pcf.pin(1), {MIDI_Notes::Ab(OCTAVE), CHORDS_CHANNEL}, Chords::Major },
  { 16        , {MIDI_Notes::Bb(OCTAVE), CHORDS_CHANNEL}, Chords::Major },
};

Bank<3> bank(1);
ManyButtonsSelector<3> selector = {
  bank,
  {
    pcf.pin(0),
    pcf.pin(1),
    16,
  }
};

Array<MIDIAddress, 3> stringNotes[3] = {
  {{ // string 1
    {MIDI_Notes::Db(OCTAVE), STRINGS_CHANNEL},
    {MIDI_Notes::Ab(OCTAVE), STRINGS_CHANNEL},
    {MIDI_Notes::Bb(OCTAVE), STRINGS_CHANNEL},
  }},
  {{ // string 2
    {MIDI_Notes::Db(OCTAVE)+1, STRINGS_CHANNEL},
    {MIDI_Notes::Ab(OCTAVE)+1, STRINGS_CHANNEL},
    {MIDI_Notes::Bb(OCTAVE)+1, STRINGS_CHANNEL},
  }},
  {{ // string 3
    {MIDI_Notes::Db(OCTAVE)+2, STRINGS_CHANNEL},
    {MIDI_Notes::Ab(OCTAVE)+2, STRINGS_CHANNEL},
    {MIDI_Notes::Bb(OCTAVE)+2, STRINGS_CHANNEL},
  }},
};

MPR121 mpr;

Bankable::ManyAddresses::NoteButton<3> strings[3] = {
  { bank, mpr.pin(0), stringNotes[0] },
  { bank, mpr.pin(1), stringNotes[1] },
  { bank, mpr.pin(2), stringNotes[2] },
};

void setup()
{
  // midi | mpipe | mididbg;
  Wire.begin(I2C_SDA, I2C_SCL);

  // Set up instruments
  midi.sendProgramChange({MIDI_PC::Rock_Organ, CHORDS_CHANNEL});
  midi.sendProgramChange({MIDI_PC::Electric_Guitar_Clean, STRINGS_CHANNEL});
  Control_Surface.begin();
}

void loop()
{
  Control_Surface.loop();
}
