#include <Arduino.h>
#include <Wire.h>

#include <Control_Surface.h>
#include "PCF8574.h"
#include "MPR121.h"

// Sometimes MPR INT pin causes infinite restart during image flashing
// Maybe pin 2 is not the best pin for INT?
constexpr ArduinoPin_t MPR_INT_PIN = 2;
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

Array<MIDIAddress, 3> stringNotes[12] = {
  {{ // string 1
    {MIDI_Notes::Db(OCTAVE)-5, STRINGS_CHANNEL},
    {MIDI_Notes::Ab(OCTAVE)-5, STRINGS_CHANNEL},
    {MIDI_Notes::Bb(OCTAVE)-5, STRINGS_CHANNEL},
  }},
  {{ // string 2
    {MIDI_Notes::Db(OCTAVE)-4, STRINGS_CHANNEL},
    {MIDI_Notes::Ab(OCTAVE)-4, STRINGS_CHANNEL},
    {MIDI_Notes::Bb(OCTAVE)-4, STRINGS_CHANNEL},
  }},
  {{ // string 3
    {MIDI_Notes::Db(OCTAVE)-3, STRINGS_CHANNEL},
    {MIDI_Notes::Ab(OCTAVE)-3, STRINGS_CHANNEL},
    {MIDI_Notes::Bb(OCTAVE)-3, STRINGS_CHANNEL},
  }},
  {{ // string 4
    {MIDI_Notes::Db(OCTAVE)-2, STRINGS_CHANNEL},
    {MIDI_Notes::Ab(OCTAVE)-2, STRINGS_CHANNEL},
    {MIDI_Notes::Bb(OCTAVE)-2, STRINGS_CHANNEL},
  }},
  {{ // string 5
    {MIDI_Notes::Db(OCTAVE)-1, STRINGS_CHANNEL},
    {MIDI_Notes::Ab(OCTAVE)-1, STRINGS_CHANNEL},
    {MIDI_Notes::Bb(OCTAVE)-1, STRINGS_CHANNEL},
  }},
  {{ // string 6
    {MIDI_Notes::Db(OCTAVE), STRINGS_CHANNEL},
    {MIDI_Notes::Ab(OCTAVE), STRINGS_CHANNEL},
    {MIDI_Notes::Bb(OCTAVE), STRINGS_CHANNEL},
  }},
  {{ // string 7
    {MIDI_Notes::Db(OCTAVE), STRINGS_CHANNEL},
    {MIDI_Notes::Ab(OCTAVE), STRINGS_CHANNEL},
    {MIDI_Notes::Bb(OCTAVE), STRINGS_CHANNEL},
  }},
  {{ // string 8
    {MIDI_Notes::Db(OCTAVE)+1, STRINGS_CHANNEL},
    {MIDI_Notes::Ab(OCTAVE)+1, STRINGS_CHANNEL},
    {MIDI_Notes::Bb(OCTAVE)+1, STRINGS_CHANNEL},
  }},
  {{ // string 9
    {MIDI_Notes::Db(OCTAVE)+2, STRINGS_CHANNEL},
    {MIDI_Notes::Ab(OCTAVE)+2, STRINGS_CHANNEL},
    {MIDI_Notes::Bb(OCTAVE)+2, STRINGS_CHANNEL},
  }},
  {{ // string 10
    {MIDI_Notes::Db(OCTAVE)+3, STRINGS_CHANNEL},
    {MIDI_Notes::Ab(OCTAVE)+3, STRINGS_CHANNEL},
    {MIDI_Notes::Bb(OCTAVE)+3, STRINGS_CHANNEL},
  }},
  {{ // string 11
    {MIDI_Notes::Db(OCTAVE)+4, STRINGS_CHANNEL},
    {MIDI_Notes::Ab(OCTAVE)+4, STRINGS_CHANNEL},
    {MIDI_Notes::Bb(OCTAVE)+4, STRINGS_CHANNEL},
  }},
  {{ // string 12
    {MIDI_Notes::Db(OCTAVE)+5, STRINGS_CHANNEL},
    {MIDI_Notes::Ab(OCTAVE)+5, STRINGS_CHANNEL},
    {MIDI_Notes::Bb(OCTAVE)+5, STRINGS_CHANNEL},
  }},
};

// Sometimes INT pin causes infinite restart during image flashing
MPR121 mpr = {MPR121::BASE_ADDRESS, MPR_INT_PIN};

Bankable::ManyAddresses::NoteButton<3> strings[12] = {
  { bank, mpr.pin(0),  stringNotes[0] },
  { bank, mpr.pin(1),  stringNotes[1] },
  { bank, mpr.pin(2),  stringNotes[2] },
  { bank, mpr.pin(3),  stringNotes[3] },
  { bank, mpr.pin(4),  stringNotes[4] },
  { bank, mpr.pin(5),  stringNotes[5] },
  { bank, mpr.pin(6),  stringNotes[6] },
  { bank, mpr.pin(7),  stringNotes[7] },
  { bank, mpr.pin(8),  stringNotes[8] },
  { bank, mpr.pin(9),  stringNotes[9] },
  { bank, mpr.pin(10), stringNotes[10] },
  { bank, mpr.pin(11), stringNotes[11] },
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
