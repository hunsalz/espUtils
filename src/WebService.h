#pragma once

#ifdef Arduino_h
// arduino is not compatible with std::vector
#undef min
#undef max
#endif
#include <vector>

#include <ArduinoLog.h> // https://github.com/thijse/Arduino-Log
#include <ESPAsyncTCP.h> // https://github.com/me-no-dev/ESPAsyncTCP/blob/master/src/ESPAsyncTCP.h
#include <ESPAsyncWebServer.h> // https://github.com/me-no-dev/ESPAsyncWebServer/blob/master/src/ESPAsyncWebServer.h
#include <ArduinoJson.h> // https://github.com/bblanchon/ArduinoJson
#include <StreamString.h> // https://github.com/esp8266/Arduino/tree/master/cores/esp8266

#include "Service.h"
#include "WebSocketListener.h"

namespace esp8266util {

  class WebService : public Service {

    public:

      WebService(uint16_t port = 80);

      ~WebService();

      bool isRunning();

      bool start();

      bool stop();

      AsyncWebServer* getWebServer();

      AsyncCallbackWebHandler& on(const char* uri, WebRequestMethodComposite method, ArRequestHandlerFunction onRequest);

      void send(AsyncWebServerRequest *request, JsonObject &json);

      void send(AsyncWebServerRequest *request, JsonArray &json);

      void send(AsyncWebServerRequest *request, StreamString stream);

      JsonArray& getDetails();

    private:

      AsyncWebServer webServer;

      std::vector<String> services;

      bool running = false;
  };
}
