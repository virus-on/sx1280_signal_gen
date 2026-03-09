#include "SignalGenerator.h"
#include "Arduino.h"

// --- Hardware Pin Definitions ---
#define RADIO_MOSI  13
#define RADIO_MISO  12
#define RADIO_SCLK  14
#define RADIO_NSS   15
#define RADIO_DIO1   4
#define RADIO_BUSY   5
#define RADIO_RST    2

SignalGenerator::SignalGenerator()
    : _mod(RADIO_NSS, RADIO_DIO1, RADIO_RST, RADIO_BUSY)
    , _radio(&_mod)
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
        _amp.applyState(AmplifierState::AMP_NONE);
        _radio.standby();
    }
    
    if (_state.frequency != inState.frequency)
    {
        inState.frequency = constrain(inState.frequency, FREQ_MIN_MHZ, FREQ_MAX_MHZ);
        _radio.setFrequency(inState.frequency);
    }
    if (_state.outputPower != inState.outputPower)
    {
        inState.outputPower = constrain(inState.outputPower, POWER_MIN_DBM, POWER_MAX_DBM);
        _radio.setOutputPower(inState.outputPower);
    }
    
    if (inState.bSignal)
    {
        if (inState.bAmp)
        {
            _amp.applyState(AmplifierState::AMP_TX);
        }
        _radio.transmitDirect();
    }

    _state = inState;
}

AmplifierController::AmplifierController()
{
    pinMode(PIN_TXE, OUTPUT);
    pinMode(PIN_RXE, OUTPUT);

    applyState(AmplifierState::AMP_NONE);
}

AmplifierController::~AmplifierController()
{
    applyState(AmplifierState::AMP_NONE);
}

void AmplifierController::applyState(AmplifierState inState)
{
    switch (inState)
    {
    case AmplifierState::AMP_NONE:
        digitalWrite(PIN_TXE, LOW);
        digitalWrite(PIN_RXE, LOW);
        delay(SAFE_SWITCH_DELAY_MS);
        break;
    case AmplifierState::AMP_RX:
        digitalWrite(PIN_TXE, LOW);
        delay(SAFE_SWITCH_DELAY_MS);
        digitalWrite(PIN_RXE, HIGH);
        break;
    case AmplifierState::AMP_TX:
        digitalWrite(PIN_RXE, LOW);
        delay(SAFE_SWITCH_DELAY_MS);
        digitalWrite(PIN_TXE, HIGH);
        break;
    }

    _state = inState;
}