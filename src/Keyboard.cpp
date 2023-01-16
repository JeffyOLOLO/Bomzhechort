#include "BcKeyboard.h"

#include <PCF8574.h>

PCF8574 pipicf[5] = {0x20, 0x21, 0x22, 0x23, 0x24};

namespace BC
{
void Keyboard::setup()
{
    for (auto &&p : pipicf)
    {
        for (auto &&pin : {P0, P1, P2, P3, P4, P5, P6, P7})
        {
            p.pinMode(pin, OUTPUT);
        }
    }
}

Scales Keyboard::read()
{
    const PCF8574::DigitalInput buttons[5] = {
        pipicf[0].digitalReadAll(),
        pipicf[1].digitalReadAll(),
        pipicf[2].digitalReadAll(),
        pipicf[3].digitalReadAll(),
        pipicf[4].digitalReadAll(),
    };

    const Scales ch = {
        { // Major
            buttons[0].p0, buttons[0].p1, buttons[0].p2, buttons[0].p3, buttons[0].p4, buttons[0].p5, buttons[0].p6, buttons[0].p7,
            buttons[1].p0, buttons[1].p1, buttons[1].p2, buttons[1].p3
        },
        { // Minor
            buttons[1].p4, buttons[1].p5, buttons[1].p6, buttons[1].p7,
            buttons[2].p0, buttons[2].p1, buttons[2].p2, buttons[2].p3, buttons[2].p4, buttons[2].p5, buttons[2].p6, buttons[2].p7
        },
        { // Seventh
            buttons[3].p0, buttons[3].p1, buttons[3].p2, buttons[3].p3, buttons[3].p4, buttons[3].p5, buttons[3].p6, buttons[3].p7,
            buttons[4].p0, buttons[4].p1, buttons[4].p2, buttons[4].p3
        }
    };

    return ch;
}
}
