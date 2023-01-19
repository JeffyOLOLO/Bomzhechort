#include "PCF8574.h"

#include <Wire.h>

BEGIN_AH_NAMESPACE

void PCF8574::pinModeBuffered(pin_t pin, PinMode_t mode)
{
    Serial.print("pinModeBuffered:\n");
    Serial.print("  pin: ");
    Serial.print(pin);
    Serial.print("\n");
    Serial.print("  pinModesDirty = ");
    Serial.print(pinModesDirty ? "true" : "false");
    Serial.print("\n");
    Serial.print("  bufferedPinModes = ");
    Serial.print(bufferedPinModes.getByte(0));
    Serial.print("\n");
    Serial.print("  mode: ");

    if (mode == INPUT || mode == INPUT_PULLUP || mode == INPUT_PULLDOWN) {
        Serial.print("  INPUT");
        pinModesDirty |= bufferedPinModes.get(pin) == PIN_MODE_OUTPUT;
        bufferedPinModes.set(pin);
    } else if (mode == OUTPUT) {
        Serial.print("  OUTPUT");
        pinModesDirty |= bufferedPinModes.get(pin) == PIN_MODE_INPUT;
        bufferedPinModes.clear(pin);
    } else {
        Serial.print("  unsupported");
    }

    Serial.print("\n");
    Serial.print("  pinModesDirty = ");
    Serial.print(pinModesDirty ? "true" : "false");
    Serial.print("\n");
    Serial.print("  bufferedPinModes = ");
    Serial.print(bufferedPinModes.getByte(0));
    Serial.print("\n");
    Serial.print("pinModeBuffered end.\n");
}

void PCF8574::begin()
{
    Serial.print("begin:\n");
    if (interruptPin != NO_PIN)
    {
        Serial.print("  INT pin processing...\n");
        ExtIO::pinMode(interruptPin, INPUT_PULLUP);
    }
    Serial.print("begin end.\n");
}

void PCF8574::digitalWriteBuffered(pin_t pin, PinStatus_t status)
{
    // Serial.print("digitalWriteBuffered:\n");
    if (bufferedPinModes.get(pin) == PIN_MODE_INPUT)
        // just get out of here if it's input pin
        return;
    const auto boolstate = status == HIGH;
    outputsDirty |= bufferedOutputs.get(pin) != boolstate;
    bufferedOutputs.set(pin, boolstate);
}

void PCF8574::analogWriteBuffered(pin_t pin, analog_t value)
{
    // Serial.print("analogWriteBuffered:\n");
    digitalWriteBuffered(pin, value >= 0x80 ? HIGH : LOW);
}

PinStatus_t PCF8574::digitalReadBuffered(pin_t pin)
{
    // Serial.print("digitalReadBuffered:\n");
    return bitRead(bufferedInputs, pin) ? LOW : HIGH;
}

analog_t PCF8574::analogReadBuffered(pin_t pin)
{
    // Serial.print("analogReadBuffered:\n");
    return bitRead(bufferedInputs, pin) ? 0 : 1023;
}

// Phisically update output pins state
void PCF8574::updateBufferedOutputs()
{
    // updateBufferedPinModes();
    // Serial.print("updateBufferedOutputs:\n");
    if (!outputsDirty && !pinModesDirty)
        // there is nothing to write to output pins
        return;
    // Set Outputs + pins that configured as Inputs to HIGH, because
    // "The I/Os should be high before being used as inputs."
    const uint8_t outputData = bufferedOutputs.getByte(0) | bufferedPinModes.getByte(0);
    // Write this byte over I²C
    Serial.print("  outputData = ");
    Serial.print(outputData, 2);
    Serial.print("\n");
    writeGPIO(outputData);
    outputsDirty = false;
    pinModesDirty = false;
    Serial.print("updateBufferedOutputs end.\n");
}

// Physical read to the inputs buffer
void PCF8574::updateBufferedInputs()
{
    // Serial.print("updateBufferedInputs:\n");
    // Only update if a pin change interrupt happened
    if (interruptPin != NO_PIN && ExtIO::digitalRead(interruptPin) == HIGH)
        return;
    bufferedInputs = readGPIO(); // read from I²C
    // Serial.print("  bufferedInputs = ");
    // Serial.print(bufferedInputs);
    // Serial.print("\n");
    // Serial.print("updateBufferedInputs end.\n");
}

// Is it not required since PCF does not require port configuration signal?
// Physical change pin modes if they were changed
void PCF8574::updateBufferedPinModes()
{
    // Serial.print("updateBufferedPinModes:\n");
    if (pinModesDirty) {
        // Set Outputs + pins that configured as Inputs to HIGH, because
        // "The I/Os should be high before being used as inputs."
        const uint8_t outputData = bufferedOutputs.getByte(0) | bufferedPinModes.getByte(0);
        // Write this byte over I²C
        writeGPIO(outputData);
        pinModesDirty = false;
    }
}

uint8_t PCF8574::readGPIO()
{
    // Serial.print("readGPIO:\n");
    Wire.requestFrom(address, size_t(1));
    return Wire.read();
}

void PCF8574::writeGPIO(uint8_t data)
{
    // Serial.print("writeGPIO:\n");
    Wire.beginTransmission(address);
    Wire.write(data);
    Wire.endTransmission();
}


END_AH_NAMESPACE
