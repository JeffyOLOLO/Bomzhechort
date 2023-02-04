#include <Arduino.h>
#include <Wire.h>

#include <Control_Surface.h>
#include "PCF8574.h"
// #include "MPR121.h"

// Sometimes MPR INT pin causes infinite restart during image flashing
// Maybe pin 2 is not the best pin for INT?
// constexpr ArduinoPin_t MPR_INT_PIN = 2;
constexpr ArduinoPin_t PCF_INT_PIN = 13;
// Didn't work with 12 and 13 pins, so do not use them for I²C
constexpr ArduinoPin_t I2C_SCL_PIN = 15;
constexpr ArduinoPin_t I2C_SDA_PIN = 14;

// BluetoothMIDI_Interface bleMidi;
USBDebugMIDI_Interface usbDbgMidi = 115200;
// // Create a MIDI pipe factory to connect the MIDI interfaces to Control Surface
// // BidirectionalMIDI_PipeFactory<2> pipes;

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wdeprecated-declarations"
using Octave_t = AH::function_traits<decltype(MIDI_Notes::note)>::argument_t<1>;
constexpr Octave_t OCTAVE = 4;
#pragma GCC diagnostic pop

constexpr auto CHORDS_CHANNEL  = CHANNEL_1;
constexpr auto STRINGS_CHANNEL = CHANNEL_2;

PCF8574 pcf[5] = {
  // PCF8574 can be 0x20-0x27
  {0x20, PCF_INT_PIN},
  // PCF8574A can be 0x38-0x3F
  {0x38, PCF_INT_PIN},
  {0x39, PCF_INT_PIN},
  {0x3A, PCF_INT_PIN},
  {0x3B, PCF_INT_PIN},
};

NoteChordButton chords[36] = {
  // Major
  { pcf[1].pin(0), {MIDI_Notes::Db(OCTAVE), CHORDS_CHANNEL}, Chords::Major },
  { pcf[1].pin(1), {MIDI_Notes::Ab(OCTAVE), CHORDS_CHANNEL}, Chords::Major },
  { pcf[1].pin(2), {MIDI_Notes::Eb(OCTAVE), CHORDS_CHANNEL}, Chords::Major },
  { pcf[1].pin(3), {MIDI_Notes::Bb(OCTAVE), CHORDS_CHANNEL}, Chords::Major },
  { pcf[1].pin(4), {MIDI_Notes::F_(OCTAVE), CHORDS_CHANNEL}, Chords::Major },
  { pcf[1].pin(5), {MIDI_Notes::C(OCTAVE),  CHORDS_CHANNEL}, Chords::Major },
  { pcf[1].pin(6), {MIDI_Notes::G(OCTAVE),  CHORDS_CHANNEL}, Chords::Major },
  { pcf[1].pin(7), {MIDI_Notes::D(OCTAVE),  CHORDS_CHANNEL}, Chords::Major },
  { pcf[0].pin(0), {MIDI_Notes::A(OCTAVE),  CHORDS_CHANNEL}, Chords::Major },
  { pcf[0].pin(2), {MIDI_Notes::E(OCTAVE),  CHORDS_CHANNEL}, Chords::Major },
  { pcf[0].pin(4), {MIDI_Notes::B(OCTAVE),  CHORDS_CHANNEL}, Chords::Major },
  { pcf[0].pin(6), {MIDI_Notes::Gb(OCTAVE), CHORDS_CHANNEL}, Chords::Major }, // F#
  // Minor
  { pcf[4].pin(6), {MIDI_Notes::Db(OCTAVE), CHORDS_CHANNEL}, Chords::Minor },
  { pcf[4].pin(4), {MIDI_Notes::Ab(OCTAVE), CHORDS_CHANNEL}, Chords::Minor },
  { pcf[4].pin(2), {MIDI_Notes::Eb(OCTAVE), CHORDS_CHANNEL}, Chords::Minor },
  { pcf[3].pin(7), {MIDI_Notes::Bb(OCTAVE), CHORDS_CHANNEL}, Chords::Minor },
  { pcf[3].pin(6), {MIDI_Notes::F_(OCTAVE), CHORDS_CHANNEL}, Chords::Minor },
  { pcf[3].pin(5), {MIDI_Notes::C(OCTAVE),  CHORDS_CHANNEL}, Chords::Minor },
  { pcf[3].pin(3), {MIDI_Notes::G(OCTAVE),  CHORDS_CHANNEL}, Chords::Minor },
  { pcf[3].pin(1), {MIDI_Notes::D(OCTAVE),  CHORDS_CHANNEL}, Chords::Minor },
  { pcf[0].pin(1), {MIDI_Notes::A(OCTAVE),  CHORDS_CHANNEL}, Chords::Minor },
  { pcf[0].pin(3), {MIDI_Notes::E(OCTAVE),  CHORDS_CHANNEL}, Chords::Minor },
  { pcf[0].pin(5), {MIDI_Notes::B(OCTAVE),  CHORDS_CHANNEL}, Chords::Minor },
  { pcf[0].pin(7), {MIDI_Notes::Gb(OCTAVE), CHORDS_CHANNEL}, Chords::Minor }, // F#
  // 7th
  { pcf[4].pin(7), {MIDI_Notes::Db(OCTAVE), CHORDS_CHANNEL}, Chords::DominantSeventh },
  { pcf[4].pin(5), {MIDI_Notes::Ab(OCTAVE), CHORDS_CHANNEL}, Chords::DominantSeventh },
  { pcf[4].pin(3), {MIDI_Notes::Eb(OCTAVE), CHORDS_CHANNEL}, Chords::DominantSeventh },
  { pcf[4].pin(1), {MIDI_Notes::Bb(OCTAVE), CHORDS_CHANNEL}, Chords::DominantSeventh },
  { pcf[4].pin(0), {MIDI_Notes::F_(OCTAVE), CHORDS_CHANNEL}, Chords::DominantSeventh },
  { pcf[3].pin(4), {MIDI_Notes::C(OCTAVE),  CHORDS_CHANNEL}, Chords::DominantSeventh },
  { pcf[3].pin(2), {MIDI_Notes::G(OCTAVE),  CHORDS_CHANNEL}, Chords::DominantSeventh },
  { pcf[3].pin(0), {MIDI_Notes::D(OCTAVE),  CHORDS_CHANNEL}, Chords::DominantSeventh },
  { pcf[2].pin(3), {MIDI_Notes::A(OCTAVE),  CHORDS_CHANNEL}, Chords::DominantSeventh },
  { pcf[2].pin(2), {MIDI_Notes::E(OCTAVE),  CHORDS_CHANNEL}, Chords::DominantSeventh },
  { pcf[2].pin(1), {MIDI_Notes::B(OCTAVE),  CHORDS_CHANNEL}, Chords::DominantSeventh },
  { pcf[2].pin(0), {MIDI_Notes::Gb(OCTAVE), CHORDS_CHANNEL}, Chords::DominantSeventh }, // F#
};

Bank<3> bank(1);
ManyButtonsSelector<3> selector = {
  bank,
  {
    pcf[0].pin(0),
    pcf[0].pin(1),
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
// MPR121 mpr = {MPR121::BASE_ADDRESS, MPR_INT_PIN};

// Bankable::ManyAddresses::NoteButton<3> strings[12] = {
//   { bank, mpr.pin(0),  stringNotes[0] },
//   { bank, mpr.pin(1),  stringNotes[1] },
//   { bank, mpr.pin(2),  stringNotes[2] },
//   { bank, mpr.pin(3),  stringNotes[3] },
//   { bank, mpr.pin(4),  stringNotes[4] },
//   { bank, mpr.pin(5),  stringNotes[5] },
//   { bank, mpr.pin(6),  stringNotes[6] },
//   { bank, mpr.pin(7),  stringNotes[7] },
//   { bank, mpr.pin(8),  stringNotes[8] },
//   { bank, mpr.pin(9),  stringNotes[9] },
//   { bank, mpr.pin(10), stringNotes[10] },
//   { bank, mpr.pin(11), stringNotes[11] },
// };

void setup()
{
  Wire.begin(I2C_SDA_PIN, I2C_SCL_PIN);

  // Control_Surface | pipes | usbDbgMidi;
  // Control_Surface | pipes | bleMidi;
  // Set up instruments
  Control_Surface.begin();
  Control_Surface.sendProgramChange({MIDI_PC::Rock_Organ, CHORDS_CHANNEL});
  Control_Surface.sendProgramChange({MIDI_PC::Electric_Guitar_Clean, STRINGS_CHANNEL});
}

void loop()
{
  Control_Surface.loop();
}
