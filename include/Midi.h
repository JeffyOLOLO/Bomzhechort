#ifndef MIDI_H
#define MIDI_H

#include <Arduino.h>

class Midi
{
public:
    virtual ~Midi();

    void virtual setup();
    void virtual noteOn(const uint8_t channel, const uint8_t note, const uint8_t velocity) const;
    void virtual noteOff(const uint8_t channel, const uint8_t note, const uint8_t velocity) const;
};

#endif // MIDI_H
