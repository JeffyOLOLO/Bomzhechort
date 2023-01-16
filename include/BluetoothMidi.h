#ifndef BLUETOOTHMIDI_H
#define BLUETOOTHMIDI_H

#include "MidiBase.h"

class BluetoothMidi: public MidiBase
{
public:
    BluetoothMidi(const std::string& device_name)
    : m_device_name(device_name)
    {}

    void setup() override;
    void noteOn(const uint8_t channel, const uint8_t note, const uint8_t velocity) const override;
    void noteOff(const uint8_t channel, const uint8_t note, const uint8_t velocity) const override;

    bool isConnected() const;
private:
    const std::string m_device_name;
};

#endif // BLUETOOTHMIDI_H