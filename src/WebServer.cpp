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
    RadioState newRadioState;
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

    _ctx.getSignalGenerator().applyState(newRadioState);

    String json = "{\"signal\":"+ String(newRadioState.bSignal ? "true" : "false") +
                ",\"freq\":"    + String(newRadioState.frequency) +
                ",\"power\":"   + String(newRadioState.outputPower) +
                ",\"clients\":" + String(_ctx.getWiFiController().getConnectedClients()) + "}";
    _server.send(200, "application/json", json);
}

void WebServer::handleStatus() 
{
    auto& radioState = _ctx.getSignalGenerator().getState();
    String json = "{\"signal\":"+ String(radioState.bSignal ? "true" : "false") +
                ",\"freq\":"    + String(radioState.frequency) +
                ",\"power\":"   + String(radioState.outputPower) +
                ",\"clients\":" + String(_ctx.getWiFiController().getConnectedClients()) + "}";
    _server.send(200, "application/json", json);
}