#pragma once

#include <ArduinoLog.h> // https://github.com/thijse/Arduino-Log
#include <ESPAsyncTCP.h> // https://github.com/me-no-dev/ESPAsyncTCP/blob/master/src/ESPAsyncTCP.h
#include <ESPAsyncWebServer.h> // https://github.com/me-no-dev/ESPAsyncWebServer/blob/master/src/ESPAsyncWebServer.h
#include <ArduinoJson.h> // https://github.com/bblanchon/ArduinoJson

#include "Service.h"
#include "WSHandler.h"

class WebService : public Service {

  public:

    WebService(); // default http port 80

    WebService(uint16_t port);

    WebService(uint16_t port, const char *rootCtx);

    ~WebService();

    bool start();

    bool stop();

    AsyncWebServer* getWebServer();

    AsyncWebSocket* addWebSocket(const String &path, WSHandler* wsHandler);

    AsyncCallbackWebHandler& on(const char* uri, WebRequestMethodComposite method, ArRequestHandlerFunction onRequest);

    void send(AsyncWebServerRequest *request, JsonObject &json);

    void send(AsyncWebServerRequest *request, JsonArray &json);

  private:

		AsyncWebServer webServer;

    const char *_rootCtx = "index.html";
};
