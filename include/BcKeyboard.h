#pragma once

#include <Arduino.h>
#include <bitset>

namespace BC
{
struct Chords
{
    uint8_t Db;
    uint8_t Ab;
    uint8_t Eb;
    uint8_t Bb;
    uint8_t F;
    uint8_t C;
    uint8_t G;
    uint8_t D;
    uint8_t A;
    uint8_t E;
    uint8_t B;
    uint8_t Fsh;
};

struct Scales
{
    Chords major;
    Chords minor;
    Chords seventh;
};

class Keyboard
{
public:
    void setup();
    Scales read();
};
}
