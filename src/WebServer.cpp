#include "WebServer.h"
#include "Context.h"

WebServer::WebServer(Context& inContext, const char* inHTMLPage)
    : _ctx(inContext)
    , _HTMLPage(inHTMLPage)
    , _server(80)
{
    _server.on("/",       [this]() { handleRoot(); });
    _server.on("/set",    [this]() { handleSet(); });
    _server.on("/status", [this]() { handleStatus(); });
    _server.begin();
    Serial.println(F("[HTTP] Web server started on port 80"));
}

void WebServer::tick(unsigned long)
{
    _server.handleClient();
}

void WebServer::handleRoot()
{
    _server.send_P(200, "text/html", _HTMLPage);

}

void WebServer::handleSet() 
{
    RadioState newRadioState = _ctx.getSignalGenerator().getState();

    if (_server.hasArg("freq")) 
    {
        newRadioState.frequency = _server.arg("freq").toInt();
    }
    if (_server.hasArg("power")) 
    {
        newRadioState.outputPower = _server.arg("power").toInt();
    }
    if (_server.hasArg("signal")) 
    {
        newRadioState.bSignal = (_server.arg("signal") == "1");
    }
    if (_server.hasArg("amp"))
    {
        newRadioState.bAmp = (_server.arg("amp") == "1");
    }

    _ctx.getSignalGenerator().applyState(newRadioState);
    
    char jsonBuf[256] = {};
    snprintf(jsonBuf, sizeof(jsonBuf), 
        "{\"signal\":%s,\"freq\":%d,\"power\":%d,\"amp\":%d,\"clients\":%d}",
        newRadioState.bSignal ? "true" : "false",
        newRadioState.frequency,
        newRadioState.outputPower,
        newRadioState.bAmp,
        _ctx.getWiFiController().getConnectedClients());
    _server.sendHeader("Cache-Control", "no-cache, no-store, must-revalidate");
    _server.send(200, "application/json", jsonBuf);
}

void WebServer::handleStatus() 
{
    auto& radioState = _ctx.getSignalGenerator().getState();
    char jsonBuf[256] = {};
    snprintf(jsonBuf, sizeof(jsonBuf), 
        "{\"signal\":%s,\"freq\":%d,\"power\":%d,\"amp\":%d,\"clients\":%d}",
        radioState.bSignal ? "true" : "false",
        radioState.frequency,
        radioState.outputPower,
        radioState.bAmp,
        _ctx.getWiFiController().getConnectedClients());
    _server.sendHeader("Cache-Control", "no-cache, no-store, must-revalidate");
    _server.send(200, "application/json", jsonBuf);
}