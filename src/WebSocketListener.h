#pragma once

#include <ArduinoLog.h> // https://github.com/thijse/Arduino-Log
#include <AsyncWebSocket.h> // https://github.com/me-no-dev/ESPAsyncWebServer/blob/master/src/AsyncWebSocket.h
#include <ArduinoJson.h> // https://github.com/bblanchon/ArduinoJson

class WebSocketListener {

  typedef std::function<void(AsyncWebSocket *ws, AsyncWebSocketClient *client, AwsEventType type, void *arg, uint8_t *data, size_t len)> EventHandler;

  public:

    void onEvent(AsyncWebSocket *ws, AsyncWebSocketClient *client, AwsEventType type, void *arg, uint8_t *data, size_t len);

    virtual void process(AsyncWebSocket *ws, AsyncWebSocketClient *client, JsonObject &json) = 0;

    void onConnect(EventHandler handler);
    void onDisconnect(EventHandler handler);
    void onError(EventHandler handler);
    void onPong(EventHandler handler);
    void onMessage(EventHandler handler);

  private:

    EventHandler connectEventHandler;
    EventHandler disconnectEventHandler;
    EventHandler errorEventHandler;
    EventHandler pongEventHandler;
    EventHandler messageEventHandler;

    void handleConnectEvent(AsyncWebSocket *ws, AsyncWebSocketClient *client, AwsEventType type, void *arg, uint8_t *data, size_t len);
    void handleDisconnectEvent(AsyncWebSocket *ws, AsyncWebSocketClient *client, AwsEventType type, void *arg, uint8_t *data, size_t len);
    void handleErrorEvent(AsyncWebSocket *ws, AsyncWebSocketClient *client, AwsEventType type, void *arg, uint8_t *data, size_t len);
    void handlePongEvent(AsyncWebSocket *ws, AsyncWebSocketClient *client, AwsEventType type, void *arg, uint8_t *data, size_t len);
    void handleMessageEvent(AsyncWebSocket *ws, AsyncWebSocketClient *client, AwsEventType type, void *arg, uint8_t *data, size_t len);
};
