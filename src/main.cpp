#include <Arduino.h>
#include <Wire.h>

#include <Control_Surface.h>
#include "PCF8574.h"

BluetoothMIDI_Interface midi;

PCF8574 pcf[5] = {0x20, 0x21, 0x22, 0x23, 0x24};

// MIDI Note middle C (C in the fourth octave):
NoteButton button = {pcf[0].pin(0), {MIDI_Notes::note(MIDI_Notes::C, 4)}};

constexpr int8_t octave = 4;
// Major
NoteChordButton Dbmajor = { pcf[0].pin(0), {MIDI_Notes::note(MIDI_Notes::Db, octave)}, Chords::Major };
NoteChordButton Abmajor = { pcf[0].pin(0), {MIDI_Notes::note(MIDI_Notes::Ab, octave)}, Chords::Major };
NoteChordButton Ebmajor = { pcf[0].pin(0), {MIDI_Notes::note(MIDI_Notes::Eb, octave)}, Chords::Major };
NoteChordButton Bbmajor = { pcf[0].pin(0), {MIDI_Notes::note(MIDI_Notes::Bb, octave)}, Chords::Major };
NoteChordButton Fmajor  = { pcf[0].pin(0), {MIDI_Notes::note(MIDI_Notes::F,  octave)}, Chords::Major };
NoteChordButton Cmajor  = { pcf[0].pin(0), {MIDI_Notes::note(MIDI_Notes::C,  octave)}, Chords::Major };
NoteChordButton Gmajor  = { pcf[0].pin(0), {MIDI_Notes::note(MIDI_Notes::G,  octave)}, Chords::Major };
NoteChordButton Dmajor  = { pcf[0].pin(0), {MIDI_Notes::note(MIDI_Notes::D,  octave)}, Chords::Major };
NoteChordButton Amajor  = { pcf[0].pin(0), {MIDI_Notes::note(MIDI_Notes::A,  octave)}, Chords::Major };
NoteChordButton Emajor  = { pcf[0].pin(0), {MIDI_Notes::note(MIDI_Notes::E,  octave)}, Chords::Major };
NoteChordButton Bmajor  = { pcf[0].pin(0), {MIDI_Notes::note(MIDI_Notes::B,  octave)}, Chords::Major };
NoteChordButton F_major = { pcf[0].pin(0), {MIDI_Notes::note(MIDI_Notes::Gb, octave)}, Chords::Major };
// Minor
NoteChordButton Dbminor = { pcf[0].pin(0), {MIDI_Notes::note(MIDI_Notes::Db, octave)}, Chords::Minor };
NoteChordButton Abminor = { pcf[0].pin(0), {MIDI_Notes::note(MIDI_Notes::Ab, octave)}, Chords::Minor };
NoteChordButton Ebminor = { pcf[0].pin(0), {MIDI_Notes::note(MIDI_Notes::Eb, octave)}, Chords::Minor };
NoteChordButton Bbminor = { pcf[0].pin(0), {MIDI_Notes::note(MIDI_Notes::Bb, octave)}, Chords::Minor };
NoteChordButton Fminor  = { pcf[0].pin(0), {MIDI_Notes::note(MIDI_Notes::F,  octave)}, Chords::Minor };
NoteChordButton Cminor  = { pcf[0].pin(0), {MIDI_Notes::note(MIDI_Notes::C,  octave)}, Chords::Minor };
NoteChordButton Gminor  = { pcf[0].pin(0), {MIDI_Notes::note(MIDI_Notes::G,  octave)}, Chords::Minor };
NoteChordButton Dminor  = { pcf[0].pin(0), {MIDI_Notes::note(MIDI_Notes::D,  octave)}, Chords::Minor };
NoteChordButton Aminor  = { pcf[0].pin(0), {MIDI_Notes::note(MIDI_Notes::A,  octave)}, Chords::Minor };
NoteChordButton Eminor  = { pcf[0].pin(0), {MIDI_Notes::note(MIDI_Notes::E,  octave)}, Chords::Minor };
NoteChordButton Bminor  = { pcf[0].pin(0), {MIDI_Notes::note(MIDI_Notes::B,  octave)}, Chords::Minor };
NoteChordButton F_minor = { pcf[0].pin(0), {MIDI_Notes::note(MIDI_Notes::Gb, octave)}, Chords::Minor };
// 7th
NoteChordButton Dbseventh = { pcf[0].pin(0), {MIDI_Notes::note(MIDI_Notes::Db, octave)}, Chords::DominantSeventh };
NoteChordButton Abseventh = { pcf[0].pin(0), {MIDI_Notes::note(MIDI_Notes::Ab, octave)}, Chords::DominantSeventh };
NoteChordButton Ebseventh = { pcf[0].pin(0), {MIDI_Notes::note(MIDI_Notes::Eb, octave)}, Chords::DominantSeventh };
NoteChordButton Bbseventh = { pcf[0].pin(0), {MIDI_Notes::note(MIDI_Notes::Bb, octave)}, Chords::DominantSeventh };
NoteChordButton Fseventh  = { pcf[0].pin(0), {MIDI_Notes::note(MIDI_Notes::F,  octave)}, Chords::DominantSeventh };
NoteChordButton Cseventh  = { pcf[0].pin(0), {MIDI_Notes::note(MIDI_Notes::C,  octave)}, Chords::DominantSeventh };
NoteChordButton Gseventh  = { pcf[0].pin(0), {MIDI_Notes::note(MIDI_Notes::G,  octave)}, Chords::DominantSeventh };
NoteChordButton Dseventh  = { pcf[0].pin(0), {MIDI_Notes::note(MIDI_Notes::D,  octave)}, Chords::DominantSeventh };
NoteChordButton Aseventh  = { pcf[0].pin(0), {MIDI_Notes::note(MIDI_Notes::A,  octave)}, Chords::DominantSeventh };
NoteChordButton Eseventh  = { pcf[0].pin(0), {MIDI_Notes::note(MIDI_Notes::E,  octave)}, Chords::DominantSeventh };
NoteChordButton Bseventh  = { pcf[0].pin(0), {MIDI_Notes::note(MIDI_Notes::B,  octave)}, Chords::DominantSeventh };
NoteChordButton F_seventh = { pcf[0].pin(0), {MIDI_Notes::note(MIDI_Notes::Gb, octave)}, Chords::DominantSeventh };

// Strings
// 36 - is the number of banks; 1 - is address offset when we select the next bank
Bank<36> bank(1);
// Set that Pin N selects Bank N
ManyButtonsSelector<36> selector = {
  bank,
  {
    // Chord pins
    pcf[0].pin(0),
    pcf[0].pin(0),
    pcf[0].pin(0),
    pcf[0].pin(0),
    pcf[0].pin(0),
    pcf[0].pin(0),
    pcf[0].pin(0),
    pcf[0].pin(0),
    pcf[0].pin(0),
    pcf[0].pin(0),
    pcf[0].pin(0),
    pcf[0].pin(0),
    pcf[0].pin(0),
    pcf[0].pin(0),
    pcf[0].pin(0),
    pcf[0].pin(0),
    pcf[0].pin(0),
    pcf[0].pin(0),
    pcf[0].pin(0),
    pcf[0].pin(0),
    pcf[0].pin(0),
    pcf[0].pin(0),
    pcf[0].pin(0),
    pcf[0].pin(0),
    pcf[0].pin(0),
    pcf[0].pin(0),
    pcf[0].pin(0),
    pcf[0].pin(0),
    pcf[0].pin(0),
    pcf[0].pin(0),
    pcf[0].pin(0),
    pcf[0].pin(0),
    pcf[0].pin(0),
    pcf[0].pin(0),
    pcf[0].pin(0),
    pcf[0].pin(0),
  }
};
// Array of all possible notes for a string
Array<MIDIAddress, 36> arr = {
  MIDI_Notes::note(MIDI_Notes::A, octave),
  MIDI_Notes::note(MIDI_Notes::B, octave),
  MIDI_Notes::note(MIDI_Notes::C, octave),
  MIDI_Notes::note(MIDI_Notes::D, octave),
  MIDI_Notes::note(MIDI_Notes::A, octave),
  MIDI_Notes::note(MIDI_Notes::B, octave),
  MIDI_Notes::note(MIDI_Notes::C, octave),
  MIDI_Notes::note(MIDI_Notes::D, octave),
  MIDI_Notes::note(MIDI_Notes::A, octave),
  MIDI_Notes::note(MIDI_Notes::B, octave),
  MIDI_Notes::note(MIDI_Notes::C, octave),
  MIDI_Notes::note(MIDI_Notes::D, octave),
  MIDI_Notes::note(MIDI_Notes::A, octave),
  MIDI_Notes::note(MIDI_Notes::B, octave),
  MIDI_Notes::note(MIDI_Notes::C, octave),
  MIDI_Notes::note(MIDI_Notes::D, octave),
  MIDI_Notes::note(MIDI_Notes::A, octave),
  MIDI_Notes::note(MIDI_Notes::B, octave),
  MIDI_Notes::note(MIDI_Notes::C, octave),
  MIDI_Notes::note(MIDI_Notes::D, octave),
  MIDI_Notes::note(MIDI_Notes::A, octave),
  MIDI_Notes::note(MIDI_Notes::B, octave),
  MIDI_Notes::note(MIDI_Notes::C, octave),
  MIDI_Notes::note(MIDI_Notes::D, octave),
  MIDI_Notes::note(MIDI_Notes::A, octave),
  MIDI_Notes::note(MIDI_Notes::B, octave),
  MIDI_Notes::note(MIDI_Notes::C, octave),
  MIDI_Notes::note(MIDI_Notes::D, octave),
  MIDI_Notes::note(MIDI_Notes::A, octave),
  MIDI_Notes::note(MIDI_Notes::B, octave),
  MIDI_Notes::note(MIDI_Notes::C, octave),
  MIDI_Notes::note(MIDI_Notes::D, octave),
  MIDI_Notes::note(MIDI_Notes::A, octave),
  MIDI_Notes::note(MIDI_Notes::B, octave),
  MIDI_Notes::note(MIDI_Notes::C, octave),
  MIDI_Notes::note(MIDI_Notes::D, octave),
};
// Bind Bank to Array and set button
Bankable::ManyAddresses::NoteButton<36> first_string = {
  bank,
  pcf[0].pin(0), // string pin
  arr
};

// Will it work w/o a Selector?
ProgramChanger<1> s = {
  {MIDI_PC::Rock_Organ},
  CHANNEL_1
};

void setup()
{
  Serial.begin(115200);
  Wire.begin();
  Control_Surface.begin();
}

void loop()
{
  Control_Surface.loop();
}
