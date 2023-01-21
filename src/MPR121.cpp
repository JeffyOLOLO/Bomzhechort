#include "MPR121.h"

#include <Wire.h>

BEGIN_AH_NAMESPACE

void MPR121::begin()
{
    Serial.print("begin:\n");
    if (interruptPin != NO_PIN)
    {
        Serial.print("  INT pin processing...\n");
        ExtIO::pinMode(interruptPin, INPUT_PULLUP);
    }

    if (!mpr.begin(BASE_ADDRESS))
      Serial.print("MPR121 init error.\n");

    Serial.print("begin end.\n");
}

PinStatus_t MPR121::digitalReadBuffered(pin_t pin)
{
    // Serial.print("digitalReadBuffered:\n");
    // Inverted logic to imitate pullup
    return bitRead(bufferedInputs, pin) ? LOW : HIGH;
}

analog_t MPR121::analogReadBuffered(pin_t pin)
{
    // Serial.print("analogReadBuffered:\n");
    return bitRead(bufferedInputs, pin) ? 0 : 1023;
}

// Physical read to the inputs buffer
void MPR121::updateBufferedInputs()
{
    // Serial.print("updateBufferedInputs:\n");
    // Only update if a pin change interrupt happened
    if (interruptPin != NO_PIN && ExtIO::digitalRead(interruptPin) == HIGH)
        return;
    bufferedInputs = mpr.touched(); // read from I²C
    // Serial.print("  bufferedInputs = ");
    // Serial.print(bufferedInputs);
    // Serial.print("\n");
    // Serial.print("updateBufferedInputs end.\n");
}

END_AH_NAMESPACE
