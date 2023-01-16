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

std::vector<uint8_t> Keyboard::read()
{
    const PCF8574::DigitalInput di[5] = {
        pcf[0].digitalReadAll(),
        pcf[1].digitalReadAll(),
        pcf[2].digitalReadAll(),
        pcf[3].digitalReadAll(),
        pcf[4].digitalReadAll(),
    };

    return {
        di[0].p0,
        di[0].p1,
        di[0].p2,
        di[0].p3,
        di[0].p4,
        di[0].p5,
        di[0].p6,
        di[0].p7,
        di[1].p0,
        di[1].p1,
        di[1].p2,
        di[1].p3,
        di[1].p4,
        di[1].p5,
        di[1].p6,
        di[1].p7,
        di[2].p0,
        di[2].p1,
        di[2].p2,
        di[2].p3,
        di[2].p4,
        di[2].p5,
        di[2].p6,
        di[2].p7,
        di[3].p0,
        di[3].p1,
        di[3].p2,
        di[3].p3,
        di[3].p4,
        di[3].p5,
        di[3].p6,
        di[3].p7,
        di[4].p0,
        di[4].p1,
        di[4].p2,
        di[4].p3,
        di[4].p4,
        di[4].p5,
        di[4].p6,
        di[4].p7,
    };
}

const uint8_t note[36] =
{
  // TODO: add notes

  // Major
  123,
  122,
  // Minor
  111,
  // Seventh
  22,
};

void Keyboard::play(const MidiBase& midi)
{
    const auto keys = this->read();
    for (size_t i = 0; i < keys.size(); i++)
    {
        if (keys[i])
        {
            midi.noteOn(0, note[keys[i]], 127);
        }
        else
        {
            midi.noteOff(0, note[keys[i]], 127);
        }
    }
}