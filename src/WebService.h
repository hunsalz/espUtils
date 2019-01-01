#ifndef WEB_SERVICE_H
#define WEB_SERVICE_H

// #ifdef Arduino_h
// // Arduino is not compatible with std::vector
// #undef min
// #undef max
// #endif
// #include <vector>

#include <ArduinoJson.h> // https://github.com/bblanchon/ArduinoJson
#include <ESPAsyncWebServer.h>  // https://github.com/me-no-dev/ESPAsyncWebServer

#include <StreamString.h>  // https://github.com/esp8266/Arduino/tree/master/cores/esp8266

#include "Logging.hpp"
#include "polyfills/Json2String.h"

namespace esp8266utils {

class WebService {
 public:
  WebService(uint16_t port = 80);

  bool begin();

  // TODO bool end();

  AsyncWebServer &getWebServer();

  uint8_t getPort();

  AsyncCallbackWebHandler &on(const char *uri, WebRequestMethodComposite method,
                              ArRequestHandlerFunction onRequest);

  AsyncCallbackWebHandler &on(const char *uri, WebRequestMethodComposite method,
                              ArRequestHandlerFunction onRequest,
                              ArBodyHandlerFunction onBody);

  AsyncCallbackWebHandler &on(const char *uri, WebRequestMethodComposite method,
                              ArRequestHandlerFunction onRequest,
                              ArBodyHandlerFunction onBody,
                              ArUploadHandlerFunction onUpload);

  void send(AsyncWebServerRequest *request, const char *type,
            const String &response);

  void send(AsyncWebServerRequest *request, JsonVariant &json);

  String getServices();

 private:
  AsyncWebServer _webServer;

  uint8_t _port = 80;

  std::vector<String> _services;
};
}  // namespace esp8266utils

#if !defined(NO_GLOBAL_INSTANCES)
extern esp8266utils::WebService SERVER;
#endif  // NO_GLOBAL_INSTANCES

#endif  // WEB_SERVICE_H
