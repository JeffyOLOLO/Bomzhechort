#ifndef PCF8574_H
#define PCF8574_H

#include <AH/Hardware/ExtendedInputOutput/StaticSizeExtendedIOElement.hpp>
#include <AH/Containers/BitArray.hpp>

BEGIN_AH_NAMESPACE

class PCF8574 : public StaticSizeExtendedIOElement<8>
{
public:
    PCF8574(uint8_t address = 0x20, pin_t interruptPin = NO_PIN)
        : address(address), interruptPin(interruptPin) {}

    void pinModeBuffered(pin_t pin, PinMode_t mode) override;
    void digitalWriteBuffered(pin_t pin, PinStatus_t status) override;
    int digitalReadBuffered(pin_t pin) override;
    analog_t analogReadBuffered(pin_t pin) override;
    void analogWriteBuffered(pin_t, analog_t) override;

    void begin() override;

    void updateBufferedOutputs() override;
    void updateBufferedInputs() override;
    void updateBufferedPinModes();

private:
    uint8_t address;
    pin_t interruptPin;

    // static bool isConnectedToI2C;

    bool pinModesDirty = true;
    BitArray<8> bufferedPinModes;
    bool pullupsDirty = true;
    BitArray<8> bufferedPullups;
    bool outputsDirty = true;
    BitArray<8> bufferedOutputs;
    uint8_t bufferedInputs = 0;
};

END_AH_NAMESPACE

#endif // PCF8574_H
