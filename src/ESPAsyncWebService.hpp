#pragma once

#include <ArduinoJson.h>        // https://github.com/bblanchon/ArduinoJson
#include <ESPAsyncTCP.h>        // https://github.com/me-no-dev/ESPAsyncTCP
#include <ESPAsyncWebServer.h>  // https://github.com/me-no-dev/ESPAsyncWebServer
#include <StreamString.h>       // https://github.com/esp8266/Arduino/tree/master/cores/esp8266

#include "Logging.hpp"

namespace esp8266utils {

class ESPAsyncWebService {
 
 public:
  
  struct Resource {
    const char* uri;
    Resource* next;
  };

  ESPAsyncWebService(uint8_t port) {
    _port = port;
  }

  ~ESPAsyncWebService() {

    // TODO cleaup any WebServer resources?

    // clean up linked resource list
    Resource* node;
    while (_resourceRoot) {
      node = _resourceRoot;
		  _resourceRoot = _resourceRoot->next;
      delete node; 
    }
  }

  bool begin() {
    
    _webServer = new AsyncWebServer(getPort());

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
    
    // find last leaf of linked resource list
    Resource* node = _resourceRoot;
    Resource* nextNode = _resourceRoot;
    while (nextNode) {
      // verify for duplicate resources
      if (strcmp(uri, nextNode->uri) == 0) {
        WARNING_FP(F("Duplicate resource uri detected: %s"), uri);
      }
      // keep current node and follow next
      node = nextNode;
      nextNode = node->next;
    }
    // create new leaf
    Resource* tmp = new Resource();
    tmp->uri = uri;
    tmp->next = nullptr;
    // add leaf at appropriate position
    if (_resourceRoot == nullptr) {
      _resourceRoot = tmp;
    } else {
      node->next = tmp;
    }
    // add handler to web server
    return getWebServer().on(uri, method, onRequest, onUpload, onBody);
  }

  size_t serializeResources(String& output) {
    
    DynamicJsonDocument doc;
    JsonArray json = doc.to<JsonArray>();
    // add linked list of resources to a json array
    Resource* node = _resourceRoot;
    while (node) {
      json.add(node->uri);
      node = node->next;
    }
    serializeJson(json, output);
    return measureJson(json);
  }

 private:
  
  AsyncWebServer* _webServer;

  uint8_t _port;

  Resource* _resourceRoot = nullptr;
};
}  // namespace esp8266utils