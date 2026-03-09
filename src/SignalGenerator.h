#pragma once

#include <RadioLib.h>

struct RadioState
{
    int frequency = 2400;
    int8 outputPower = 10;
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
    static constexpr int PIN_RXE = 9; // LNA
    static constexpr int PIN_TXE = 10; // PA
    static constexpr int SAFE_SWITCH_DELAY_MS = 1;
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

    static constexpr int POWER_MIN_DBM = -17;
    static constexpr int POWER_MAX_DBM = 12;
    static constexpr int FREQ_MIN_MHZ = 2400;
    static constexpr int FREQ_MAX_MHZ = 2500;
};