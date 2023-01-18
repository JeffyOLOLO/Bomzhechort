#ifndef PCF8574_H
#define PCF8574_H

#include <AH/Hardware/ExtendedInputOutput/StaticSizeExtendedIOElement.hpp>
#include <AH/Containers/BitArray.hpp>

BEGIN_AH_NAMESPACE

class PCF8574 : public StaticSizeExtendedIOElement<8>
{
public:
    constexpr static uint8_t BASE_ADDRESS = 0x20;

    PCF8574(uint8_t address = BASE_ADDRESS, pin_t interruptPin = NO_PIN)
        : address(address), interruptPin(interruptPin) {}

    void pinModeBuffered(pin_t pin, PinMode_t mode) override;
    void digitalWriteBuffered(pin_t pin, PinStatus_t status) override;
    PinStatus_t digitalReadBuffered(pin_t pin) override;
    analog_t analogReadBuffered(pin_t pin) override;
    void analogWriteBuffered(pin_t, analog_t) override;

    void begin() override;

    void updateBufferedOutputs() override;
    void updateBufferedInputs() override;
    void updateBufferedPinModes();

protected:
    uint8_t readGPIO();
    void writeGPIO(uint8_t data);

private:
    const uint8_t address;
    const pin_t interruptPin;

    // mode of some pin(s) was changed
    bool pinModesDirty = true;
    constexpr static bool PIN_MODE_INPUT  = 1;
    constexpr static bool PIN_MODE_OUTPUT = 0;
    // 0 - output; 1 - input
    BitArray<8> bufferedPinModes;

    // state of some pin(s) was changed
    bool outputsDirty = true;
    // 0 - will set pin to low; 1 - will set pin to high
    BitArray<8> bufferedOutputs;

    // state of pins is stored to this buffer
    uint8_t bufferedInputs = 0;
};

END_AH_NAMESPACE

#endif // PCF8574_H
