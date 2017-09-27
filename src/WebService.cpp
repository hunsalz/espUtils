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

//AsyncWebSocket* WebService::addWebSocket(const String &path, WebSocketListener* wsListener) {

  //AsyncWebSocket* webSocket = new AsyncWebSocket(path);

  //AwsEventHandler awsEventHandler = [wsListener](AsyncWebSocket *ws, AsyncWebSocketClient *client, AwsEventType type, void *arg, uint8_t *data, size_t len) {
    //Log.verbose(F("LALA ............ ws[%s][%u] received : %d bytes\n" CR), ws->url(), client->id(), len);
  //  wsListener->process(ws, client, type, arg, data, len);
  //};

  //webSocket->onEvent(awsEventHandler);

  //webSocket->onEvent(std::bind(&WebSocketListener::onEvent, (WebSocketListener*)wsListener, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4, std::placeholders::_5, std::placeholders::_6));
  //webServer.addHandler(webSocket);

  //return webSocket;
//}

AsyncCallbackWebHandler& WebService::on(const char* uri, WebRequestMethodComposite method, ArRequestHandlerFunction onRequest) {

  // TODO service registry

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
