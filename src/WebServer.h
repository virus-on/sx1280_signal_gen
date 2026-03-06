#pragma once

#include <ESP8266WebServer.h>

class Context;

class WebServer
{
public:
    WebServer(Context& inContext, const char* inHTMLPage);

    void tick(unsigned long);

private:
    void handleRoot();
    void handleSet();
    void handleStatus();


private:
    Context& _ctx;
    const char* _HTMLPage;
    ESP8266WebServer _server;
};