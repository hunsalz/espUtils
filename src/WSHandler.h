#pragma once

#include <ArduinoLog.h> // https://github.com/thijse/Arduino-Log
#include <AsyncWebSocket.h> // https://github.com/me-no-dev/ESPAsyncWebServer/blob/master/src/AsyncWebSocket.h
#include <ArduinoJson.h> // https://github.com/bblanchon/ArduinoJson

class WSHandler {

  public:

    void onEvent(AsyncWebSocket *ws, AsyncWebSocketClient *client, AwsEventType type, void *arg, uint8_t *data, size_t len);

    virtual void processMessage(AsyncWebSocket *ws, AsyncWebSocketClient *client, JsonObject &json) = 0;

    virtual void notify(AsyncWebSocket *ws, AsyncWebSocketClient *client, String type, bool broadcast) = 0;

    // TODO support JsonArray in send method

    void send(AsyncWebSocket *ws, AsyncWebSocketClient *client, JsonObject *json, bool broadcast);
};
