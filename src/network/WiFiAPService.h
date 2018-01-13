#ifndef WIFIAPSERVICE_H
#define WIFIAPSERVICE_H

#ifdef ESP8266
extern "C" {
#include "user_interface.h" // Expressif ESP8266 Non-OS API
}
#endif

#include <ESP8266WiFi.h> // https://github.com/esp8266/Arduino/blob/master/libraries/ESP8266WiFi/src/ESP8266WiFi.h
#include <Log4Esp.h>     // https://github.com/hunsalz/log4Esp
// include ArduinoJson.h last - order of includes matters
#include <ArduinoJson.h> // https://github.com/bblanchon/ArduinoJson

#include "MDNSService.h"
#include "Service.h"

using esp8266util::MDNS_SERVICE;
using log4Esp::LOG;

namespace esp8266util {

class WiFiAPService : public Service {

public:
  ~WiFiAPService();

  bool available();

  bool begin(const char *ssid, const char *passphrase, int channel = 1, int ssid_hidden = 0, int max_connection = 5, bool autoConnect = true, bool persistent = false);

  bool end();

  ESP8266WiFiClass &getWiFi();

  JsonObject &getDetails();

  String macAddress(const unsigned char *mac);

private:
  WiFiEventHandler _softAPModeStationConnectedHandler;
  WiFiEventHandler _softAPModeStationDisconnectedHandler;
  WiFiEventHandler _softAPModeProbeRequestReceivedHandler;
};

extern WiFiAPService WIFI_STATION;
} // namespace esp8266util

#endif // WIFIAPSERVICE_H
