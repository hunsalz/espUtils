#ifndef WEBSERVICE_H
#define WEBSERVICE_H

#ifdef Arduino_h
// Arduino is not compatible with std::vector
#undef min
#undef max
#endif
#include <vector>

#include <ArduinoJson.h>       // https://github.com/bblanchon/ArduinoJson
#include <ESPAsyncWebServer.h> // https://github.com/me-no-dev/ESPAsyncWebServer
#include <Log4Esp.h>           // https://github.com/hunsalz/log4Esp
#include <StreamString.h>      // https://github.com/esp8266/Arduino/tree/master/cores/esp8266

#include "JsonHelper.h"
#include "Service.h"
#include "WebSocketListener.h"

using esp8266util::toString;
using log4Esp::LOG;

namespace esp8266util {

class WebService : public Service {

public:
  ~WebService();

  bool available();

  bool begin(uint16_t port = 80);

  // TODO bool end();

  AsyncWebServer &getWebServer();

  uint8_t getPort();

  AsyncCallbackWebHandler &on(const char *uri, WebRequestMethodComposite method, ArRequestHandlerFunction onRequest);

  AsyncCallbackWebHandler &on(const char *uri, WebRequestMethodComposite method, ArRequestHandlerFunction onRequest, ArBodyHandlerFunction onBody);

  AsyncCallbackWebHandler &on(const char *uri, WebRequestMethodComposite method, ArRequestHandlerFunction onRequest, ArBodyHandlerFunction onBody, ArUploadHandlerFunction onUpload);

  void send(AsyncWebServerRequest *request, JsonObject &json);

  void send(AsyncWebServerRequest *request, JsonArray &json);

  void send(AsyncWebServerRequest *request, StreamString stream);

  JsonArray &getDetails();

private:
  AsyncWebServer *_webServer = NULL;

  uint8_t _port = 80;

  std::vector<String> _services;
};

extern WebService SERVER;
} // namespace esp8266util

#endif // WEBSERVICE_H
