#include "LedController.h"

#include "Config.h"
#include "WiFiController.h"
#include "Context.h"

#include <Arduino.h>

LedController::LedController(Context& inContext)
        : _ctx(inContext)
{
    pinMode(LED_PIN, OUTPUT);
    digitalWrite(LED_PIN, HIGH);
}

void LedController::tick(unsigned long delta)
{
    auto newMode = determineMode();

    if (_ledMode != newMode)
    {
        digitalWrite(LED_PIN, LOW);
        _cumulativeDelta = 0;
        _ledMode = newMode;
    }
    else
    {
        _cumulativeDelta += delta;
    }

    switch (_ledMode)
    {
        case LedController::LedMode::LED_BREATHE:
            breathe();
            return;
        case LedController::LedMode::LED_BLINK:
            blink();
            return;
        default:
            digitalWrite(LED_PIN, HIGH);
    }
}

LedController::LedMode LedController::determineMode()
{
    if (_ctx.getSignalGenerator().getState().bSignal)
    {
        return LedMode::LED_SOLID;
    }
    else if (_ctx.getWiFiController().getConnectedClients() > 0)
    {
        return LedMode::LED_BREATHE;
    }
    else
    {
        return LedMode::LED_BLINK;
    }
}

void LedController::blink()
{
    if (_cumulativeDelta >= BLINK_PERIOD_MS)
    {
        _bBlinkState = !_bBlinkState;
        digitalWrite(LED_PIN, _bBlinkState ? LOW : HIGH);
        _cumulativeDelta %= BLINK_PERIOD_MS;
    }
}

void LedController::breathe()
{
    if (_cumulativeDelta >= BREATH_STEP_MS)
    {
        analogWrite(LED_PIN, LED_MAX - _breathBrightness);
        if (_bBreathDirection) 
        {
            if (++_breathBrightness > LED_MAX)
            {
                _bBreathDirection = false;
            }
        } 
        else 
        {
            if (--_breathBrightness < 0)
            {
                _bBreathDirection = true;
            }
        }
        _cumulativeDelta %= BREATH_STEP_MS;
    }
}

