#pragma once

// #ifdef Arduino_h
// // Arduino is not compatible with std::vector
// #undef min
// #undef max
// #endif
// #include <vector>

#include <ArduinoJson.h>        // https://github.com/bblanchon/ArduinoJson
#include <ESPAsyncWebServer.h>  // https://github.com/me-no-dev/ESPAsyncWebServer

#include <StreamString.h>  // https://github.com/esp8266/Arduino/tree/master/cores/esp8266

#include "Logging.hpp"
#include "polyfills/Json2String.h"

namespace esp8266utils {

class WebService {
 
 public:
  
  bool begin() {
    
    // add generic services registry resource
    on("/services", HTTP_GET, [this](AsyncWebServerRequest *request) {
      send(request, "text/json", getServices());
    });
    // add default 404 handler
    getWebServer().onNotFound([this](AsyncWebServerRequest *request) {
      VERBOSE_MSG_P(F("HTTP 404 : [http://%s%s] not found."), request->host().c_str(), request->url().c_str());

      // request->send(404, "text/plain", F("404 error - Page not found."));

      // _  _    ___  _  _
      //| || |  / _ \| || |
      //| || |_| | | | || |_
      //|__   _| | | |__   _|
      //   | | | |_| |  | |
      //   |_|  \___/   |_| page not found

      request->send(404, "text/plain", F(" _  _    ___  _  _\n| || |  / _ \\| || |\n| || |_| | | | || |_\n|__   _| | | |__   _|\n   | | | |_| |  | |\n   |_|  \\___/   |_| page not found"));
    });

    // TODO SSL/TLS example:
    // https://github.com/me-no-dev/ESPAsyncWebServer/issues/75 Works, but isn't
    // reliable:
    // https://github.com/esp8266/Arduino/issues/2733#issuecomment-264710234
    // getWebServer().onSslFileRequest([](void * arg, const char *filename,
    // uint8_t **buf) -> int {
    //   File file = SPIFFS.open(filename, "r");
    //   if (file) {
    //     Serial.printf("SSL file found: %s\n", filename);
    //     size_t size = file.size();
    //     uint8_t * nbuf = (uint8_t*)malloc(size);
    //     if (nbuf) {
    //       size = file.read(nbuf, size);
    //       file.close();
    //       *buf = nbuf;
    //       return size;
    //     }
    //     file.close();
    //   } else {
    //     Serial.printf("SSL file not found: %s\n", filename);
    //   }
    //   *buf = 0;
    //   return 0;
    // }, NULL);
    // getWebServer().beginSecure("/server.cer", "/server.key", NULL);

    // start web server
    _webServer.begin();

    VERBOSE_MSG_P(F("WebServer started."));
    return true;  // TODO
  }

  AsyncWebServer& getWebServer() {
    // FIXME
    return _webServer;
  }

  uint8_t getPort() {
    return _port;
  }

  AsyncCallbackWebHandler& on(const char *uri, WebRequestMethodComposite method, ArRequestHandlerFunction onRequest) {
    // (Caution) overwrites & disables "catchAllHandler" for onRequestBody(...)
    // in ESPAsyncWebServer; maybe rework again
    on(uri, method, onRequest, [](AsyncWebServerRequest *request, uint8_t *data, size_t len, size_t index, size_t total) {},
      [](AsyncWebServerRequest *request, const String &filename, size_t index, uint8_t *data, size_t len, bool final) {});
  }

  AsyncCallbackWebHandler& on(const char *uri, WebRequestMethodComposite method, ArRequestHandlerFunction onRequest, ArBodyHandlerFunction onBody) {
    
    on(uri, method, onRequest, onBody,
      [](AsyncWebServerRequest *request, const String &filename, size_t index, uint8_t *data, size_t len, bool final) {});
  }

  AsyncCallbackWebHandler& on(const char *uri, WebRequestMethodComposite method, ArRequestHandlerFunction onRequest, ArBodyHandlerFunction onBody, ArUploadHandlerFunction onUpload) {
    
    // add uri to service listing
    _services.push_back(String(uri));
    // add to web server
    getWebServer().on(uri, method, onRequest, onUpload, onBody);
  }

  void send(AsyncWebServerRequest *request, const char *type, const String &response) {
    
    VERBOSE_MSG_P(F("Send %s response: %s"), type, response.c_str());
    request->send(new AsyncBasicResponse(200, "text/json", response));
  }

  void send(AsyncWebServerRequest *request, JsonVariant &json) {
    
    String response = esp8266utils::toString(json);
    VERBOSE_MSG_P(F("Send text/json response: %s"), response.c_str());
    request->send(new AsyncBasicResponse(200, "text/json", response));
  }

  String getServices() {
    
    DynamicJsonDocument doc;
    JsonArray array = doc.to<JsonArray>();
    for (std::vector<String>::iterator i = _services.begin();
         i != _services.end(); ++i) {
      array.add(*i);
    }
    return esp8266utils::toString(array);
  }

 private:
  
  AsyncWebServer _webServer;

  uint8_t _port = 80;

  std::vector<String> _services;
};
}  // namespace esp8266utils

#if !defined(NO_GLOBAL_INSTANCES)
extern esp8266utils::WebService SERVER;
#endif  // NO_GLOBAL_INSTANCES