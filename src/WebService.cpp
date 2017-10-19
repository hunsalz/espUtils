#include "WebService.h"

namespace esp8266util {

  WebService::WebService(uint16_t port) :
    webServer(port) {
  }

  WebService::~WebService() {
    stop();
  }

  bool WebService::isRunning() {
    return running;
  }

  bool WebService::start() {

    if (!isRunning()) {
      // TODO make TLS available
      // webServer.onSslFileRequest(...)
      // example: https://github.com/me-no-dev/ESPAsyncWebServer/issues/75


      // add default 404 handler
      webServer.onNotFound([](AsyncWebServerRequest *request) {
        Log.verbose(F("HTTP 404 : [http://%s%s] not found." CR), request->host().c_str(), request->url().c_str());
        request->send(404, "text/plain", F("Page not found."));
        // TODO retest
      });
      // add generic services resource
      on("/services", HTTP_GET, [this](AsyncWebServerRequest *request) {
        send(request, getDetails());
      });
      // start web server
      webServer.begin();

      // TODO SSL
      // webServer.onSslFileRequest([](void * arg, const char *filename, uint8_t **buf) -> int {
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
      // webServer.beginSecure("/server.cer", "/server.key", NULL);

      running = true;
    }

    Log.verbose("WebServer started.\n");

    return isRunning();
  }

  bool WebService::stop() {

    if (isRunning()) {
      webServer.reset();
    }

    return isRunning();
  }

  AsyncWebServer* WebService::getWebServer() {
    return &webServer;
  }

  AsyncCallbackWebHandler& WebService::on(const char* uri, WebRequestMethodComposite method, ArRequestHandlerFunction onRequest) {

    // add uri to service listing
    services.push_back(String(uri));

    webServer.on(uri, method, onRequest);
  }

  // TODO JSON -> char[]*

  void WebService::send(AsyncWebServerRequest *request, JsonObject &json) {

    int length = json.measureLength() + 1;
    char content[length];
    json.printTo(content, length);
    Log.verbose(F("Send response: %s." CR), content);

    request->send(new AsyncBasicResponse(200, "text/json", String(content)));
  }

  void WebService::send(AsyncWebServerRequest *request, JsonArray &json) {

    int length = json.measureLength() + 1;
    char content[length];
    json.printTo(content, length);
    Log.verbose(F("Send response: %s." CR), content);

    request->send(new AsyncBasicResponse(200, "text/json", String(content)));
  }

  JsonArray& WebService::getDetails() {

    DynamicJsonBuffer jsonBuffer;
    JsonArray& json = jsonBuffer.createArray();
    for(std::vector<String>::iterator i = services.begin(); i != services.end(); ++i) {
      json.add(*i);
    }

    return json;
  }
}
