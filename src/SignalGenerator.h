#pragma once

#include "Config.h"

#include <RadioLib.h>

struct RadioState
{
    int frequency = FREQ_MIN_MHZ;
    int8 outputPower = POWER_MIN_DBM;
    bool bSignal = false;
    bool bAmp = false;

    bool operator==(const RadioState& rhs)
    {
        return rhs.bSignal == bSignal
            && rhs.bAmp == bAmp
            && rhs.frequency == frequency
            && rhs.outputPower == outputPower;
    }
};

enum class AmplifierState
{
    AMP_NONE,
    AMP_TX,
    AMP_RX
};

class AmplifierController
{
public:
    AmplifierController();
    ~AmplifierController();
    void applyState(AmplifierState inState);

private:
    AmplifierState _state = AmplifierState::AMP_NONE;

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
    AmplifierController _amp;
    Module _mod;
    SX1280 _radio;
    RadioState _state;


};