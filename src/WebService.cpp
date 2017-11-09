#include "WebService.h"

namespace esp8266util {

  WebService::WebService(uint16_t port) :
    _webServer(port) {
  }

  WebService::~WebService() {
    stop();
  }

  bool WebService::isRunning() {
    return _running;
  }

  bool WebService::start() {

    if (!isRunning()) {
      // TODO make TLS available
      // _webServer.onSslFileRequest(...)
      // example: https://github.com/me-no-dev/ESPAsyncWebServer/issues/75

      // add generic services registry resource
      on("/services", HTTP_GET, [this](AsyncWebServerRequest *request) {
        send(request, getDetails());
      });

      // add default 404 handler
      _webServer.onNotFound([this](AsyncWebServerRequest *request) {
        Log.verbose(F("HTTP 404 : [http://%s%s] not found." CR), request->host().c_str(), request->url().c_str());
        request->send(404, "text/plain", F("Page not found."));
        // TODO retest
      });

      // TODO SSL
      // _webServer.onSslFileRequest([](void * arg, const char *filename, uint8_t **buf) -> int {
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
      // _webServer.beginSecure("/server.cer", "/server.key", NULL);

      // start web server
      _webServer.begin();

      _running = true;
    }

    Log.verbose("WebServer started.\n");

    return isRunning();
  }

  bool WebService::stop() {

    if (isRunning()) {
      _webServer.reset();
    }

    return isRunning();
  }

  AsyncWebServer* WebService::getWebServer() {
    return &_webServer;
  }

  AsyncCallbackWebHandler& WebService::on(const char* uri, WebRequestMethodComposite method, ArRequestHandlerFunction onRequest) {
    
    // (Caution) overwrites & disables "catchAllHandler" for onRequestBody(...) in ESPAsyncWebServer; maybe rework again
    on(uri, method, onRequest, 
      [](AsyncWebServerRequest *request, uint8_t *data, size_t len, size_t index, size_t total) {},
      [](AsyncWebServerRequest *request, const String& filename, size_t index, uint8_t *data, size_t len, bool final) {});
  }

  AsyncCallbackWebHandler& WebService::on(const char* uri, WebRequestMethodComposite method, ArRequestHandlerFunction onRequest, ArBodyHandlerFunction onBody) {
    on(uri, method, onRequest, onBody,
      [](AsyncWebServerRequest *request, const String& filename, size_t index, uint8_t *data, size_t len, bool final) {});
  }
  
  AsyncCallbackWebHandler& WebService::on(const char* uri, WebRequestMethodComposite method, ArRequestHandlerFunction onRequest, ArBodyHandlerFunction onBody, ArUploadHandlerFunction onUpload) {
    
    // add uri to service listing
    _services.push_back(String(uri));
        
    _webServer.on(uri, method, onRequest, onUpload, onBody);
  }
  
  void WebService::send(AsyncWebServerRequest *request, JsonObject &json) {

    StreamString stream;
    json.prettyPrintTo(stream);
    send(request, stream);
  }

  void WebService::send(AsyncWebServerRequest *request, JsonArray &json) {

    StreamString stream;
    json.prettyPrintTo(stream);
    send(request, stream);
  }

  void WebService::send(AsyncWebServerRequest *request, StreamString stream) {
  
    request->send(new AsyncBasicResponse(200, "text/json", stream));
    Log.verbose(F("Send response: %s." CR), stream.c_str());
  }

  JsonArray& WebService::getDetails() {

    DynamicJsonBuffer jsonBuffer;
    JsonArray& json = jsonBuffer.createArray();
    for(std::vector<String>::iterator i = _services.begin(); i != _services.end(); ++i) {
      json.add(*i);
    }

    return json;
  }
}
