#include "WebService.h"

namespace esp8266utils {

WebService::WebService(uint16_t port) : _webServer(port) {}

bool WebService::begin() {
  // add generic services registry resource
  on("/services", HTTP_GET, [this](AsyncWebServerRequest *request) { 
    send(request, "text/json", getServices()); 
  });
  // add default 404 handler
  getWebServer().onNotFound([this](AsyncWebServerRequest *request) {
    LOG.verbose(F("HTTP 404 : [http://%s%s] not found."), request->host().c_str(), request->url().c_str());
    // TODO make a nice 404 page
    // request->send(404, "text/plain", F("404 error - Page not found."));

    // _  _    ___  _  _
    //| || |  / _ \| || |
    //| || |_| | | | || |_
    //|__   _| | | |__   _|
    //   | | | |_| |  | |
    //   |_|  \___/   |_|
    request->send(404, "text/plain", F(" _  _    ___  _  _\n| || |  / _ \\| || |\n| || |_| | | | || |_\n|__   _| | | |__   _|\n   | | | |_| |  | |\n   |_|  \\___/   |_|"));
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

  LOG.verbose(F("WebServer started."));

  return true;  // TODO
}

AsyncWebServer &WebService::getWebServer() {
  // FIXME
  return _webServer;
}

uint8_t WebService::getPort() {
  return _port;
}

AsyncCallbackWebHandler &WebService::on(const char *uri,
                                        WebRequestMethodComposite method,
                                        ArRequestHandlerFunction onRequest) {
  // (Caution) overwrites & disables "catchAllHandler" for onRequestBody(...) in
  // ESPAsyncWebServer; maybe rework again
  on(uri, method, onRequest,
     [](AsyncWebServerRequest *request, uint8_t *data, size_t len, size_t index,
        size_t total) {},
     [](AsyncWebServerRequest *request, const String &filename, size_t index,
        uint8_t *data, size_t len, bool final) {});
}

AsyncCallbackWebHandler &WebService::on(const char *uri,
                                        WebRequestMethodComposite method,
                                        ArRequestHandlerFunction onRequest,
                                        ArBodyHandlerFunction onBody) {
  on(uri, method, onRequest, onBody,
     [](AsyncWebServerRequest *request, const String &filename, size_t index,
        uint8_t *data, size_t len, bool final) {});
}

AsyncCallbackWebHandler &WebService::on(const char *uri,
                                        WebRequestMethodComposite method,
                                        ArRequestHandlerFunction onRequest,
                                        ArBodyHandlerFunction onBody,
                                        ArUploadHandlerFunction onUpload) {
  // add uri to service listing
  _services.push_back(String(uri));
  // add to web server
  getWebServer().on(uri, method, onRequest, onUpload, onBody);
}

void WebService::send(AsyncWebServerRequest *request, const char* type, const String& response) {

  LOG.verbose(F("Send %s response: %s"), type, response.c_str());
  request->send(new AsyncBasicResponse(200, "text/json", response));
}

void WebService::send(AsyncWebServerRequest *request, JsonVariant &json) {
  
  String response = esp8266utils::toString(json);
  LOG.verbose(F("Send text/json response: %s"), response.c_str());
  request->send(new AsyncBasicResponse(200, "text/json", response));
}

String WebService::getServices() {
  
  DynamicJsonBuffer jsonBuffer;
  JsonArray &json = jsonBuffer.createArray();
  for (std::vector<String>::iterator i = _services.begin(); i != _services.end(); ++i) {
    json.add(*i);
  }

  return esp8266utils::toString(json);
}
}  // namespace esp8266utils

#if !defined(NO_GLOBAL_INSTANCES)
esp8266utils::WebService SERVER = esp8266utils::WebService(80);
#endif  // NO_GLOBAL_INSTANCES