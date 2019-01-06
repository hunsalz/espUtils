#pragma once

#ifdef Arduino_h
// Arduino is not compatible with std::vector
#undef min
#undef max
#endif

#include <ArduinoJson.h>        // https://github.com/bblanchon/ArduinoJson
#include <ESPAsyncTCP.h>        // https://github.com/me-no-dev/ESPAsyncTCP
#include <ESPAsyncWebServer.h>  // https://github.com/me-no-dev/ESPAsyncWebServer
#include <StreamString.h>       // https://github.com/esp8266/Arduino/tree/master/cores/esp8266

#include <vector>

#include "Logging.hpp"

namespace esp8266utils {

class WebService {
 
 public:
  
  WebService(uint8_t port) {
    _port = port;
  }

  bool begin() {
    
    _webServer = new AsyncWebServer(80);

    // add generic services registry resource
    on("/resources", HTTP_GET, [this](AsyncWebServerRequest *request) {
      
      AsyncResponseStream *response = request->beginResponseStream("application/json");  
      StreamString* payload = new StreamString();
      size_t size = serializeResources(*payload);
      response->print(*payload); 
      request->send(response);
      delete payload;
    });
    // add default 404 handler
    getWebServer().onNotFound([this](AsyncWebServerRequest *request) {
      VERBOSE_FP(F("HTTP 404 : [http://%s%s] not found."), request->host().c_str(), request->url().c_str());

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
    _webServer->begin();

    VERBOSE_FP(F("WebServer started."));
    return true;  // TODO
  }

  AsyncWebServer& getWebServer() {
    return *_webServer;
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
    
    // add uri to resource listing
    _resources.push_back(String(uri));
    // add to web server
    getWebServer().on(uri, method, onRequest, onUpload, onBody);
  }

  size_t serializeResources(String& output) {
    
    // FIXME
    DynamicJsonDocument doc;
    JsonArray json = doc.to<JsonArray>();
    for (std::vector<String>::iterator i = _resources.begin(); i != _resources.end(); ++i) {
      json.add(*i);
    }
    serializeJson(json, output);
    return measureJson(json);
  }

 private:
  
  AsyncWebServer* _webServer;

  uint8_t _port;

  std::vector<String> _resources;
};
}  // namespace esp8266utils

#if !defined(NO_GLOBAL_INSTANCES)
extern esp8266utils::WebService WEBSERVICE;
#endif  // NO_GLOBAL_INSTANCES