#pragma once

#include <ESPAsyncTCP.h> // https://github.com/me-no-dev/ESPAsyncTCP/blob/master/src/ESPAsyncTCP.h
#include <ESPAsyncWebServer.h> // https://github.com/me-no-dev/ESPAsyncWebServer/blob/master/src/ESPAsyncWebServer.h
#include <AsyncJson.h> // https://github.com/me-no-dev/ESPAsyncWebServer/blob/master/src/AsyncJson.h

#include "Service.h"

class WebService : public Service {

  public:

    WebService(); // default http port 80

    WebService(uint16_t port);

    WebService(uint16_t port, const char *rootCtx);

    ~WebService();

    bool start();

    bool stop();

    AsyncWebServer* getWebServer();

  private:

		AsyncWebServer webServer;

    const char *_rootCtx = "index.html";
    bool _running = false;
};
