#ifndef WIFI_AP_SERVICE_H
#define WIFI_AP_SERVICE_H

#include <ESP8266WiFi.h> // https://github.com/esp8266/Arduino/blob/master/libraries/ESP8266WiFi/src/ESP8266WiFi.h

#include "../Logging.h"
#include "polyfills/Json2String.h"

#include <ArduinoJson.h> // https://github.com/bblanchon/ArduinoJson

namespace esp8266utils {

class WiFiAPConfig {

public:
  bool begin(const char *ssid, const char *passphrase, int channel = 1, int ssid_hidden = 0, int max_connection = 5, bool autoConnect = true, bool persistent = false);

  void end();

  WiFiEventHandler onSoftAPModeStationConnected(std::function<void(const WiFiEventSoftAPModeStationConnected&)> f);

  WiFiEventHandler onSoftAPModeStationDisconnected(std::function<void(const WiFiEventSoftAPModeStationDisconnected&)> f);

  WiFiEventHandler onSoftAPModeProbeRequestReceived(std::function<void(const WiFiEventSoftAPModeProbeRequestReceived&)> f);

  String getDetails();

private:
  WiFiEventHandler _softAPModeStationConnectedHandler;
  WiFiEventHandler _softAPModeStationDisconnectedHandler;
  WiFiEventHandler _softAPModeProbeRequestReceivedHandler;

  String macAddress(const unsigned char *mac);
};
} // namespace esp8266utils

#if !defined(NO_GLOBAL_INSTANCES)
extern esp8266utils::WiFiAPConfig WIFI_AP_CFG;
#endif // NO_GLOBAL_INSTANCES

#endif // WIFI_AP_SERVICE_H
