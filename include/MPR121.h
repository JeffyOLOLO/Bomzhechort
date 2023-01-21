#ifndef MPR121_H
#define MPR121_H

#include <AH/Hardware/ExtendedInputOutput/StaticSizeExtendedIOElement.hpp>
#include <AH/Containers/BitArray.hpp>
#include <Adafruit_MPR121.h>

BEGIN_AH_NAMESPACE

class MPR121 : public StaticSizeExtendedIOElement<12>
{
public:
    constexpr static uint8_t BASE_ADDRESS = 0x5A;
    constexpr static uint8_t DEFAULT_TOUCH_THRESHOLD = 12;
    constexpr static uint8_t DEFAULT_RELEASE_THRESHOLD  = 6;

    MPR121(uint8_t address = BASE_ADDRESS, pin_t interruptPin = NO_PIN,
        uint8_t touchThreshold = DEFAULT_TOUCH_THRESHOLD, uint8_t releaseThreshold = DEFAULT_RELEASE_THRESHOLD)
        : address(address), interruptPin(interruptPin) {}

    void pinModeBuffered(pin_t pin, PinMode_t mode) override {};
    void digitalWriteBuffered(pin_t pin, PinStatus_t status) override {};
    void analogWriteBuffered(pin_t, analog_t) override {};
    PinStatus_t digitalReadBuffered(pin_t pin) override;
    analog_t analogReadBuffered(pin_t pin) override;

    // Wire.begin() should be called in main
    void begin() override;

    void updateBufferedOutputs() override {};
    void updateBufferedInputs() override;

private:
    const uint8_t address;
    const pin_t interruptPin;
    uint8_t touchThreshold;
    uint8_t releaseThreshold;

    // state of pins is stored to this buffer
    // 0 - low; 1 - high
    uint16_t bufferedInputs;

    Adafruit_MPR121 mpr;
};

END_AH_NAMESPACE

#endif // MPR121_H
