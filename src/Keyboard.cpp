#include "Keyboard.h"

#include <PCF8574.h>

PCF8574 pcf[5] = {0x20, 0x21, 0x22, 0x23, 0x24};

void Keyboard::setup()
{
    for (auto &&p : pcf)
    {
        for (auto &&pin : {P0, P1, P2, P3, P4, P5, P6, P7})
        {
            p.pinMode(pin, OUTPUT);
        }
    }
}

Chords Keyboard::read()
{
    const PCF8574::DigitalInput buttons[5] = {
        pcf[0].digitalReadAll(),
        pcf[1].digitalReadAll(),
        pcf[2].digitalReadAll(),
        pcf[3].digitalReadAll(),
        pcf[4].digitalReadAll(),
    };

    Chords ch;
    ch.major = {
        buttons[0].p0 | buttons[0].p1 | buttons[0].p2 | buttons[0].p3 | buttons[0].p4 | buttons[0].p5 | buttons[0].p6 | buttons[0].p7
      | buttons[1].p0 | buttons[1].p1 | buttons[1].p2 | buttons[1].p3
    };

    // uint64_t ch;
    // ch |= (static_cast<Chord>(buttons[0].p0) << Chord::Db) | (buttons[0].p1 << Chord::D));
}