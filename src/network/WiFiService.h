#ifndef WIFISERVICE_H
#define WIFISERVICE_H

#include <ESP8266WiFi.h>      // https://github.com/esp8266/Arduino/blob/master/libraries/ESP8266WiFi/src/ESP8266WiFi.h
#include <ESP8266WiFiMulti.h> // https://github.com/esp8266/Arduino/tree/master/libraries/ESP8266WiFi/src/ESP8266WiFiMulti.h
#include <Log4Esp.h>          // https://github.com/hunsalz/log4Esp
#include <ArduinoJson.h>      // https://github.com/bblanchon/ArduinoJson

#include "Service.h"
#include "MDNSService.h"

using esp8266util::MDNS_SERVICE;
using log4Esp::LOG;

namespace esp8266util
{

class WiFiService : public Service
{

public:
  ~WiFiService();

  bool available();

  bool begin(uint8_t retries = 25, bool autoConnect = true, bool persistent = false);

  bool end();

  ESP8266WiFiClass &getWiFi();

  ESP8266WiFiMulti &getWiFiMulti();

  JsonObject &getDetails();

private:
  ESP8266WiFiMulti _wifiMulti;

  WiFiEventHandler _stationModeConnectedHandler;
  WiFiEventHandler _stationModeDisconnectedHandler;
  WiFiEventHandler _stationModeAuthModeChangedHandler;
  WiFiEventHandler _stationModeGotIPHandler;
  WiFiEventHandler _stationModeDHCPTimeoutHandler;
};

extern WiFiService WIFI_CLIENT;
}

#endif // WIFISERVICE_H
