#pragma once

#include <ESP8266WiFi.h>


class WiFiController
{
public:
    WiFiController(const char* SSID, const char* password = "")
    {
        WiFi.mode(WIFI_AP);
        WiFi.setOutputPower(6.f); // So that enough power remains for sx1280
        WiFi.softAP(SSID, password);
        Serial.printf("[WiFi] AP \"%s\" started. IP: ", SSID);
        Serial.println(WiFi.softAPIP());
    }

    int getConnectedClients()
    {
        return _connectedClients;
    }

    void tick(unsigned long delta)
    {
        _cumulativeDelta += delta;

        if (_cumulativeDelta >= CLIENT_CHECK_INTERVAL)
        {
            _connectedClients = WiFi.softAPgetStationNum();
            _cumulativeDelta %= CLIENT_CHECK_INTERVAL;
        }
    }

private:
    int _connectedClients = 0;
    unsigned long _cumulativeDelta = 0;

    static constexpr unsigned long CLIENT_CHECK_INTERVAL = 500;
};
