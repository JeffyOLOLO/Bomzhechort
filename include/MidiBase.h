#ifndef MIDIBASE_H
#define MIDIBASE_H

#include <Arduino.h>

class MidiBase
{
public:
    virtual ~MidiBase() {};

    void virtual setup() = 0;
    void virtual noteOn(const uint8_t channel, const uint8_t note, const uint8_t velocity) const = 0;
    void virtual noteOff(const uint8_t channel, const uint8_t note, const uint8_t velocity) const = 0;
};

#endif // MIDIBASE_H
