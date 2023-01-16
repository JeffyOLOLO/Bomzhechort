#include "PCF8574.h"

#include <Wire.h>

BEGIN_AH_NAMESPACE

bool PCF8574::isConnectedToI2C = false;

void PCF8574::pinModeBuffered(pin_t pin, PinMode_t mode) {
    if (mode == INPUT) {
        pinModesDirty |= bufferedPinModes.get(pin) == 1;
        pullupsDirty |= bufferedPullups.get(pin) == 1;
        bufferedPinModes.clear(pin);
        bufferedPullups.clear(pin);
    } else if (mode == OUTPUT) {
        pinModesDirty |= bufferedPinModes.get(pin) == 0;
        bufferedPinModes.set(pin);
    } else if (mode == INPUT_PULLUP) {
        pinModesDirty |= bufferedPinModes.get(pin) == 1;
        pullupsDirty |= bufferedPullups.get(pin) == 0;
        bufferedPinModes.clear(pin);
        bufferedPullups.set(pin);
    }
}

void PCF8574::digitalWriteBuffered(pin_t pin, PinStatus_t status) {
    bool boolstate = status == HIGH;
    outputsDirty |= bufferedOutputs.get(pin) != boolstate;
    bufferedOutputs.set(pin, boolstate);
}

int PCF8574::digitalReadBuffered(pin_t pin) {
    return bitRead(bufferedInputs, pin) ? HIGH : LOW;
}

analog_t PCF8574::analogReadBuffered(pin_t pin) {
    return bitRead(bufferedInputs, pin) ? 1023 : 0;
}

void PCF8574::analogWriteBuffered(pin_t pin, analog_t value) {
    digitalWriteBuffered(pin, value >= 0x80 ? HIGH : LOW);
}

void PCF8574::begin() {
    if (interruptPin != NO_PIN)
        ExtIO::pinMode(interruptPin, INPUT_PULLUP);
    // I²C setup if necessary
    // TODO: is this flag required?
    if (!isConnectedToI2C)
    {
        Wire.begin();
        isConnectedToI2C = true;
    }
}

void PCF8574::updateBufferedOutputs() {
    updateBufferedPinModes();
    if (!outputsDirty)
        return;
    uint8_t outputData = bufferedOutputs.getByte(0);
    // Write this byte over I²C
    Wire.beginTransmission(address);
    Wire.write(outputData);
    Wire.endTransmission();
    outputsDirty = false;
}

void PCF8574::updateBufferedInputs() {
    // Only update if a pin change interrupt happened
    if (interruptPin != NO_PIN && ExtIO::digitalRead(interruptPin) == HIGH)
        return;
    Wire.beginTransmission(address);
    bufferedInputs = Wire.read(); // read from I²C
    Wire.endTransmission();
}

void PCF8574::updateBufferedPinModes() {
    if (pinModesDirty) {
        uint8_t outputData = bufferedPinModes.getByte(0);
        // Write this byte over I²C
        Wire.beginTransmission(address);
        Wire.write(outputData);
        Wire.endTransmission();
        pinModesDirty = false;
    }
    if (pullupsDirty) {
        uint8_t outputData = bufferedPullups.getByte(0);
        // Write this byte over I²C
        Wire.beginTransmission(address);
        Wire.write(outputData);
        Wire.endTransmission();
        pullupsDirty = false;
    }
}

END_AH_NAMESPACE
