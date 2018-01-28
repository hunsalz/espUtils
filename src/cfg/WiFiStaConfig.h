#ifndef WIFI_STA_CONFIG_H
#define WIFI_STA_CONFIG_H

#include <ESP8266WiFi.h>      // https://github.com/esp8266/Arduino/blob/master/libraries/ESP8266WiFi/src/ESP8266WiFi.h
#include <ESP8266WiFiMulti.h> // https://github.com/esp8266/Arduino/tree/master/libraries/ESP8266WiFi/src/ESP8266WiFiMulti.h
#include <Log4Esp.h>          // https://github.com/hunsalz/log4Esp

#include <ArduinoJson.h> // https://github.com/bblanchon/ArduinoJson

namespace esp8266util {

class WiFiStaConfig {

public:

  bool begin(uint8_t retries = 25, bool autoConnect = true, bool persistent = false);

  void end();

  bool addAP(const char *ssid, const char *passphrase);

  WiFiEventHandler onStationModeConnected(std::function<void(const WiFiEventStationModeConnected &)> f);

  WiFiEventHandler onStationModeDisconnected(std::function<void(const WiFiEventStationModeDisconnected &)> f);

  WiFiEventHandler onStationModeAuthModeChanged(std::function<void(const WiFiEventStationModeAuthModeChanged &)> f);

  WiFiEventHandler onStationModeGotIP(std::function<void(const WiFiEventStationModeGotIP &)> f);

  WiFiEventHandler onStationModeDHCPTimeout(std::function<void(void)> f);

  JsonObject &getDetails();

private:
  ESP8266WiFiMulti _wifiMulti;

  WiFiEventHandler _stationModeConnectedHandler;
  WiFiEventHandler _stationModeDisconnectedHandler;
  WiFiEventHandler _stationModeAuthModeChangedHandler;
  WiFiEventHandler _stationModeGotIPHandler;
  WiFiEventHandler _stationModeDHCPTimeoutHandler;
};
} // namespace esp8266util

#if !defined(NO_GLOBAL_INSTANCES)
extern esp8266util::WiFiStaConfig WIFI_STA_CFG;
#endif // NO_GLOBAL_INSTANCES

#endif // WIFI_STA_CONFIG_H
