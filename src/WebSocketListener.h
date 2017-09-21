#pragma once

#include <ArduinoLog.h> // https://github.com/thijse/Arduino-Log
#include <AsyncWebSocket.h> // https://github.com/me-no-dev/ESPAsyncWebServer/blob/master/src/AsyncWebSocket.h
#include <ArduinoJson.h> // https://github.com/bblanchon/ArduinoJson

typedef std::function<void(AsyncWebSocket *ws, AsyncWebSocketClient *client, AwsEventType type, AwsFrameInfo *info, uint8_t *data, size_t len)> WSEventHandler;
typedef std::function<void(AsyncWebSocket *ws, AsyncWebSocketClient *client, AwsEventType type, uint16_t *arg, uint8_t *data, size_t len)> WSErrorHandler;


class WebSocketListener {

  public:

    void onEvent(AsyncWebSocket *ws, AsyncWebSocketClient *client, AwsEventType type, void *arg, uint8_t *data, size_t len);

    void onConnect(WSEventHandler handler);
    void onDisconnect(WSEventHandler handler);
    void onError(WSErrorHandler handler);
    void onPong(WSEventHandler handler);
    void onTextMessage(WSEventHandler handler);
    void onBinaryMessage(WSEventHandler handler);

  private:

    WSEventHandler connectWSEventHandler;
    WSEventHandler disconnectWSEventHandler;
    WSErrorHandler errorWSEventHandler;
    WSEventHandler pongWSEventHandler;
    WSEventHandler textWSEventHandler;
    WSEventHandler binaryWSEventHandler;

    void handleConnectEvent(AsyncWebSocket *ws, AsyncWebSocketClient *client, AwsEventType type, AwsFrameInfo *info, uint8_t *data, size_t len);
    void handleDisconnectEvent(AsyncWebSocket *ws, AsyncWebSocketClient *client, AwsEventType type, AwsFrameInfo *info, uint8_t *data, size_t len);
    void handleErrorEvent(AsyncWebSocket *ws, AsyncWebSocketClient *client, AwsEventType type, uint16_t *arg, uint8_t *data, size_t len);
    void handlePongEvent(AsyncWebSocket *ws, AsyncWebSocketClient *client, AwsEventType type, AwsFrameInfo *info, uint8_t *data, size_t len);
    void handleTextMessageEvent(AsyncWebSocket *ws, AsyncWebSocketClient *client, AwsEventType type, AwsFrameInfo *info, uint8_t *data, size_t len);
    void handleBinaryMessageEvent(AsyncWebSocket *ws, AsyncWebSocketClient *client, AwsEventType type, AwsFrameInfo *info, uint8_t *data, size_t len);
};
