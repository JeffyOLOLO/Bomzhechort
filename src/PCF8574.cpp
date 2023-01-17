#include "PCF8574.h"

#include <Wire.h>

BEGIN_AH_NAMESPACE

void PCF8574::pinModeBuffered(pin_t pin, PinMode_t mode) {
    if (mode == INPUT) {
        pinModesDirty |= bufferedPinModes.get(pin) == PIN_MODE_OUTPUT;
        pullupsDirty |= bufferedPullups.get(pin) == PULLUP_ON;
        bufferedPinModes.clear(pin);
        bufferedPullups.clear(pin);
    } else if (mode == OUTPUT) {
        pinModesDirty |= bufferedPinModes.get(pin) == PIN_MODE_INPUT;
        bufferedPinModes.set(pin);
    } else if (mode == INPUT_PULLUP) {
        pinModesDirty |= bufferedPinModes.get(pin) == PIN_MODE_OUTPUT;
        pullupsDirty |= bufferedPullups.get(pin) == PULLUP_OFF;
        bufferedPinModes.clear(pin);
        bufferedPullups.set(pin);
    }
}

void PCF8574::begin() {
    if (interruptPin != NO_PIN)
        ExtIO::pinMode(interruptPin, INPUT_PULLUP);
}

void PCF8574::digitalWriteBuffered(pin_t pin, PinStatus_t status) {
    if (bufferedPinModes.get(pin) == PIN_MODE_INPUT)
        // just get out of here if it's input pin
        return;
    const auto boolstate = status == HIGH;
    outputsDirty |= bufferedOutputs.get(pin) != boolstate;
    bufferedOutputs.set(pin, boolstate);
}

void PCF8574::analogWriteBuffered(pin_t pin, analog_t value) {
    digitalWriteBuffered(pin, value >= 0x80 ? HIGH : LOW);
}

PinStatus_t PCF8574::digitalReadBuffered(pin_t pin) {
    return bitRead(bufferedInputs, pin) ? HIGH : LOW;
}

analog_t PCF8574::analogReadBuffered(pin_t pin) {
    return bitRead(bufferedInputs, pin) ? 1023 : 0;
}

// Phisically update output pins state
void PCF8574::updateBufferedOutputs() {
    updateBufferedPinModes();
    if (!outputsDirty)
        // there is nothing to write to output pins
        return;
    uint8_t outputData = bufferedOutputs.getByte(0);
    // Write this byte over I²C
    writeGPIO(outputData);
    outputsDirty = false;
}

// Physical read to the inputs buffer
void PCF8574::updateBufferedInputs() {
    updateBufferedPinModes();
    // Only update if a pin change interrupt happened
    if (interruptPin != NO_PIN && ExtIO::digitalRead(interruptPin) == HIGH)
        return;
    bufferedInputs = readGPIO(); // read from I²C
}

// Physical change pin modes if they were changed
void PCF8574::updateBufferedPinModes() {
    if (pinModesDirty) {
        const auto outputData = bufferedPinModes.getByte(0);
        // Write this byte over I²C
        writeGPIO(outputData);
        pinModesDirty = false;
    }
    if (pullupsDirty) {
        const auto outputData = bufferedPullups.getByte(0);
        // Write this byte over I²C
        writeGPIO(outputData);
        pullupsDirty = false;
    }
}

uint8_t PCF8574::readGPIO()
{
    Wire.beginTransmission(address);
    const uint8_t data = Wire.read();
    Wire.endTransmission();
    return data;
}

void PCF8574::writeGPIO(uint8_t data)
{
    Wire.beginTransmission(address);
    Wire.write(data);
    Wire.endTransmission();
}


END_AH_NAMESPACE
