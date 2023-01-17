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
    uint8_t address;
    pin_t interruptPin;

    // mode of some pin(s) was changed
    bool pinModesDirty = true;
    constexpr static bool PIN_MODE_INPUT  = 0;
    constexpr static bool PIN_MODE_OUTPUT = 1;
    // 0 - input; 1 - output
    BitArray<8> bufferedPinModes;

    // pullup of some pin(s) was changed
    bool pullupsDirty = true;
    constexpr static bool PULLUP_OFF = 0;
    constexpr static bool PULLUP_ON  = 1;
    // 0 - off; 1 - on
    BitArray<8> bufferedPullups;

    // state of some pin(s) was changed
    bool outputsDirty = true;
    // 0 - will set pin to low; 1 - will set pin to high
    BitArray<8> bufferedOutputs;

    // state of pins is stored to this buffer
    uint8_t bufferedInputs = 0;
};

END_AH_NAMESPACE

#endif // PCF8574_H
