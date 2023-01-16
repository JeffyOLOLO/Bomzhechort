#pragma once

#include <Arduino.h>

namespace BC
{
class Midi
{
public:
    virtual ~Midi();

    void virtual setup() = 0;
    void virtual noteOn(const uint8_t channel, const uint8_t note, const uint8_t velocity) const = 0;
    void virtual noteOff(const uint8_t channel, const uint8_t note, const uint8_t velocity) const = 0;
};
}
