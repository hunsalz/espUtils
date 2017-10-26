#pragma once

#include <ArduinoLog.h> // https://github.com/thijse/Arduino-Log
#include <ESP8266WiFi.h> // https://github.com/esp8266/Arduino/blob/master/libraries/ESP8266WiFi/src/ESP8266WiFi.h
#include <ESP8266WiFiMulti.h> // https://github.com/esp8266/Arduino/tree/master/libraries/ESP8266WiFi/src/ESP8266WiFiMulti.h
#include <ArduinoJson.h> // https://github.com/bblanchon/ArduinoJson

#include "Service.h"

namespace esp8266util {

  class WiFiService : public Service {

    public:

      ~WiFiService();

      bool isSetup();

      bool isRunning();

      bool start();

      bool stop();

      ESP8266WiFiClass* getWiFi();

      ESP8266WiFiMulti* getWiFiMulti();

      bool setup(uint8_t retries = 20, bool autoConnect = true, bool persistent = false);

      JsonObject& getDetails();

    private:

      ESP8266WiFiMulti wifiMulti;

      byte retries = 0;

      bool setupDone = false;
  };
}
