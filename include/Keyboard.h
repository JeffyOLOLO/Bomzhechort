#ifndef KEYBOARD_H
#define KEYBOARD_H

#include <Arduino.h>
#include <bitset>

enum class Chord: uint16_t
{
    Db  = (1 << 0),
    Ab  = (1 << 1),
    Eb  = (1 << 2),
    Bb  = (1 << 3),
    F   = (1 << 4),
    C   = (1 << 5),
    G   = (1 << 6),
    D   = (1 << 7),
    A   = (1 << 8),
    E   = (1 << 9),
    B   = (1 << 10),
    Fsh = (1 << 11),
};

#ifdef ESP32
union Chord1
{
    uint16_t Db: 1,
    Ab: 1,
    Eb: 1,
    F: 1,
    C: 1,
    G: 1,
    D: 1,
    A: 1,
    E: 1,
    B: 1,
    Fsh: 1,
    :4;
    // uint8_t major;
    // uint8_t minor;
    // uint8_t seventh;
};
#else
#error Unsupported platform.
#endif

struct Chords
{
    std::bitset<12> major;
    std::bitset<12> minor;
    std::bitset<12> seventh;
};

class Keyboard
{
public:
    void setup();
    Chords read();
};

#endif // KEYBOARD_H
