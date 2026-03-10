#pragma once

#include "Config.h"
#include "SignalGenerator.h"
#include "WiFiController.h"
#include "WebServer.h"
#include "LedController.h"

#include <memory>

class Context 
{
public:
    Context(const char* inHTMLPage)
    : _webServer(std::make_unique<WebServer>(*this, inHTMLPage))
    , _ledController(std::make_unique<LedController>(*this))
    , _signalGenerator(std::make_unique<SignalGenerator>())
    , _wifiController(std::make_unique<WiFiController>(WIFI_SSID, WIFI_PASSWORD))
    {}

public:
    WebServer& getWebServer() 
    { 
        return *_webServer;
    }
    LedController& getLedController()
    {
        return *_ledController;
    }
    SignalGenerator& getSignalGenerator()
    {
        return *_signalGenerator;
    }
    WiFiController& getWiFiController()
    {
        return *_wifiController;
    }

private:
    std::unique_ptr<WebServer> _webServer;
    std::unique_ptr<LedController> _ledController;
    std::unique_ptr<SignalGenerator> _signalGenerator;
    std::unique_ptr<WiFiController> _wifiController;
};