#include <Arduino.h>

#include <Control_Surface.h>

constexpr ArduinoPin_t ORANGE = 12;
constexpr ArduinoPin_t YELOW  = 13;
constexpr ArduinoPin_t GREEN  = 15;
constexpr ArduinoPin_t BLUE   = 14;

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wdeprecated-declarations"
using Octave_t = AH::function_traits<decltype(MIDI_Notes::note)>::argument_t<1>;
constexpr Octave_t OCTAVE = 4;
#pragma GCC diagnostic pop

constexpr auto CHORDS_CHANNEL  = CHANNEL_1;
constexpr auto STRINGS_CHANNEL = CHANNEL_2;

BluetoothMIDI_Interface midi;
// USBDebugMIDI_Interface mididbg = 115200;
// BidirectionalMIDI_Pipe mpipe;

NoteChordButton Dbmajor = { ORANGE, {MIDI_Notes::Db(OCTAVE), CHORDS_CHANNEL}, Chords::Major };
NoteChordButton Abmajor = { YELOW,  {MIDI_Notes::Ab(OCTAVE), CHORDS_CHANNEL}, Chords::Major };

Bank<2> bank(1);
ManyButtonsSelector<2> selector = {
  bank,
  {
    ORANGE,
    YELOW,
  }
};

Array<MIDIAddress, 2> stringNotes[2] = {
  {{ // string 1
    {MIDI_Notes::Db(OCTAVE), STRINGS_CHANNEL},
    {MIDI_Notes::Ab(OCTAVE), STRINGS_CHANNEL},
  }},
  {{ // string 2
    {MIDI_Notes::Db(OCTAVE)+1, STRINGS_CHANNEL},
    {MIDI_Notes::Ab(OCTAVE)+1, STRINGS_CHANNEL},
  }},
};

Bankable::ManyAddresses::NoteButton<2> strings[2] = {
  { bank, GREEN, stringNotes[0] },
  { bank, BLUE,  stringNotes[1] },
};

void setup()
{
  // midi | mpipe | mididbg;
  // Set up instruments
  midi.sendProgramChange({MIDI_PC::Rock_Organ, CHORDS_CHANNEL});
  midi.sendProgramChange({MIDI_PC::Electric_Guitar_Clean, STRINGS_CHANNEL});
  Control_Surface.begin();
}

void loop()
{
  Control_Surface.loop();
}
