#pragma once

#include <RadioLib.h>

struct RadioState
{
    int frequency = 2400;
    int8 outputPower = 10;
    bool bSignal = false;

    bool operator==(RadioState& rhs)
    {
        return rhs.bSignal == bSignal 
            && rhs.frequency == frequency
            && rhs.outputPower == outputPower;
    }
};

class SignalGenerator 
{
public:
    SignalGenerator();

    ~SignalGenerator()
    {
        _radio.reset();
        SPI.end();
    }

    void applyState(RadioState& inState);

    const RadioState& getState()
    {
        return _state;
    }

private:
    SX1280 _radio;
    RadioState _state;

    static constexpr int POWER_MIN_DBM = -17;
    static constexpr int POWER_MAX_DBM = 12;
    static constexpr int FREQ_MIN_MHZ = 2400;
    static constexpr int FREQ_MAX_MHZ = 2500;
};