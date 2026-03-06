#pragma once

class Context;

class LedController
{
    enum class LedMode { LED_BLINK, LED_BREATHE, LED_SOLID };

public:
    LedController(Context& inContext);

    void tick(unsigned long delta);

private:
    LedMode determineMode();

    void blink();
    void breathe();

private:
    Context& _ctx;

    unsigned long _cumulativeDelta = 0;
    LedMode _ledMode = LedMode::LED_BLINK;

    bool _bBlinkState = false; 
    
    int _breathBrightness = 0;
    bool _bBreathDirection = true;

    static constexpr int BLINK_PERIOD_MS= 500;
    static constexpr int BREATH_STEP_MS = 3;
    static constexpr int LED_PIN = 16;
    static constexpr int LED_MAX = 255;
};