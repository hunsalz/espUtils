#include "WebService.h"

WebService::WebService(uint16_t port, const char *rootCtx) :
  webServer(port) {

  // rewrite root context
  webServer.rewrite("/", rootCtx);
}

WebService::~WebService() {
  stop();
}

bool WebService::start() {

  if (!isRunning()) {
    // handle static web resources
    webServer.serveStatic("/", SPIFFS, "/www/", "max-age:600"); // cache-control 600 seconds
    // handle 404
    webServer.onNotFound([](AsyncWebServerRequest *request) {
      String method = F("UNKNOWN");
      if (request->method() == HTTP_GET)
        method = F("GET");
      else if (request->method() == HTTP_POST)
        method = F("POST");
      else if (request->method() == HTTP_DELETE)
        method = F("DELETE");
      else if (request->method() == HTTP_PUT)
        method = F("PUT");
      else if (request->method() == HTTP_PATCH)
        method = F("PATCH");
      else if (request->method() == HTTP_HEAD)
        method = F("HEAD");
      else if (request->method() == HTTP_OPTIONS)
        method = F("OPTIONS");

      Log.verbose(F("HTTP 404 : http://%s%s" CR), request->host().c_str(), request->url().c_str());

      request->send(404, F("Page not found."));
    });
    // add generic services resource
    on("/services", HTTP_GET, [this](AsyncWebServerRequest *request) {
      send(request, getDetails());
    });
    // start web server
    webServer.begin();
    running = true;
  }
  Log.verbose("WebServer started.\n");

  return true;
}

bool WebService::stop() {

  if (isRunning()) {
    webServer.reset();

    // TODO stop webServer ?

    running = false;
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

// TODO merge send functions; problem no common base class exists between JsonObject and JsonArray

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
