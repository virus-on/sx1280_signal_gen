#include "SignalGenerator.h"

// --- Hardware Pin Definitions ---
#define RADIO_MOSI  13
#define RADIO_MISO  12
#define RADIO_SCLK  14
#define RADIO_NSS   15
#define RADIO_DIO1   4
#define RADIO_BUSY   5
#define RADIO_RST    2

SignalGenerator::SignalGenerator()
    : _radio(new Module(RADIO_NSS, RADIO_DIO1, RADIO_RST, RADIO_BUSY))
{
    SPI.begin();
    Serial.print(F("[SX1280] Initializing GFSK... "));
    int state = _radio.beginGFSK(_state.frequency, 800, 400.0, _state.outputPower);

    if (state == RADIOLIB_ERR_NONE) 
    {
        _radio.standby();
        Serial.println(F("OK — standby, waiting for web command."));
    } else {
        Serial.print(F("FAILED, code "));
        Serial.println(state);
        while(1); // stop and hang
    }
}

void SignalGenerator::applyState(RadioState& inState)
{
    if (inState == _state)
    {
        return;
    }

    if (_state.bSignal)
    {
        _radio.standby();
    }
    
    if (_state.frequency != inState.frequency)
    {
        constrain(inState.frequency, FREQ_MIN_MHZ, FREQ_MAX_MHZ);
        _radio.setFrequency(inState.frequency);
    }
    if (_state.outputPower != inState.outputPower)
    {
        constrain(inState.outputPower, FREQ_MIN_MHZ, FREQ_MAX_MHZ);
        _radio.setOutputPower(inState.outputPower);
    }
    if (inState.bSignal)
    {
        _radio.transmitDirect();
    }

    _state = inState;
}