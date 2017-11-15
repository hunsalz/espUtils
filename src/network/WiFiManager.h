#ifndef WIFIMANAGER_H
#define WIFIMANAGER_H

#include <ArduinoLog.h> // https://github.com/thijse/Arduino-Log
#include <ESP8266WiFi.h> // https://github.com/esp8266/Arduino/blob/master/libraries/ESP8266WiFi/src/ESP8266WiFi.h
#include <ESP8266WiFiMulti.h> // https://github.com/esp8266/Arduino/tree/master/libraries/ESP8266WiFi/src/ESP8266WiFiMulti.h
#include <ArduinoJson.h> // https://github.com/bblanchon/ArduinoJson

#include "Service.h"

namespace esp8266util {

  class WiFiManager : public Service {

    public:

      ~WiFiManager();

      bool available();

      bool begin(uint8_t retries = 20, bool autoConnect = true, bool persistent = false);

      ESP8266WiFiClass& getWiFi();

      ESP8266WiFiMulti& getWiFiMulti();

      JsonObject& getDetails();

    private:

      ESP8266WiFiMulti _wifiMulti;

      byte _retries = 0;

      // WiFi handlers
      WiFiEventHandler _stationModeConnectedHandler;
      WiFiEventHandler _stationModeDisconnectedHandler;
      WiFiEventHandler _stationModeAuthModeChangedHandler;
      WiFiEventHandler _stationModeGotIPHandler;
      WiFiEventHandler _stationModeDHCPTimeoutHandler;
      // WiFi station handlers
      WiFiEventHandler _softAPModeStationConnectedHandler;
      WiFiEventHandler _softAPModeStationDisconnectedHandler;
      WiFiEventHandler _softAPModeProbeRequestReceivedHandler;
  };
}

#endif // WIFIMANAGER_H
