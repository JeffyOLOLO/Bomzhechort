#ifndef KEYBOARD_H
#define KEYBOARD_H

#include <Arduino.h>
#include <vector>
#include "MidiBase.h"

class Keyboard
{
public:
    void setup();
    std::vector<uint8_t> read();
    void play(const MidiBase&);
};

#endif // KEYBOARD_H
