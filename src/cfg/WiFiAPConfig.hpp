#pragma once

#include <ArduinoJson.h>  // https://github.com/bblanchon/ArduinoJson
#include <ESP8266WiFi.h>  // https://github.com/esp8266/Arduino/blob/master/libraries/ESP8266WiFi/src/ESP8266WiFi.h

#include "../Logging.hpp"
#include "polyfills/Json2String.h"

namespace esp8266utils {

class WiFiAPConfig {
 
 public:
  
  bool begin(const char *ssid, 
    
    const char *passphrase,
    int channel = 1, 
    int ssid_hidden = 0, 
    int max_connection = 5,
    bool autoConnect = true, 
    bool persistent = false) {
    
    WiFi.enableAP(true);
    WiFi.setAutoConnect(true);
    WiFi.persistent(false);
    WiFi.softAPdisconnect();
    // add default verbose callback handlers if no handler is set
    if (!_softAPModeStationConnectedHandler) {
      onSoftAPModeStationConnected([&](const WiFiEventSoftAPModeStationConnected &event) {
        VERBOSE_MSG_P(F("MAC address [%s] joined AP."), macAddress(event.mac).c_str());
      });
    }
    if (!_softAPModeStationDisconnectedHandler) {
      onSoftAPModeStationDisconnected([&](const WiFiEventSoftAPModeStationDisconnected &event) {
        VERBOSE_MSG_P(F("MAC address [%s] disappeared from AP."), macAddress(event.mac).c_str());
      });
    }
    if (!_softAPModeProbeRequestReceivedHandler) {
      onSoftAPModeProbeRequestReceived([&](const WiFiEventSoftAPModeProbeRequestReceived &event) {
        TRACE_MSG_P(F("RSSI is [%d]"), event.rssi);
      });
    }
    // enable AP
    if (WiFi.softAP(ssid, passphrase, channel, ssid_hidden, max_connection)) {
      VERBOSE_MSG_P(F("Soft AP established successful. IP address of AP is: %s"), WiFi.softAPIP().toString().c_str());
    }

    return WiFi.isConnected();
  }

  void end() {
    WiFi.softAPdisconnect();
  }

  WiFiEventHandler onSoftAPModeStationConnected(std::function<void(const WiFiEventSoftAPModeStationConnected &)> f) {
    
    _softAPModeStationConnectedHandler = WiFi.onSoftAPModeStationConnected(f);
    return _softAPModeStationConnectedHandler;
  }

  WiFiEventHandler onSoftAPModeStationDisconnected(std::function<void(const WiFiEventSoftAPModeStationDisconnected &)> f) {
    
    _softAPModeStationDisconnectedHandler = WiFi.onSoftAPModeStationDisconnected(f);
    return _softAPModeStationDisconnectedHandler;
  }

  WiFiEventHandler onSoftAPModeProbeRequestReceived(std::function<void(const WiFiEventSoftAPModeProbeRequestReceived &)> f) {
    
    _softAPModeProbeRequestReceivedHandler = WiFi.onSoftAPModeProbeRequestReceived(f);
    return _softAPModeProbeRequestReceivedHandler;
  }

  String getDetails() {
    
    DynamicJsonDocument doc;
    JsonObject object = doc.to<JsonObject>();
    object[F("softAPgetStationNum")] = WiFi.softAPgetStationNum();
    object[F("softAPIP")] = WiFi.softAPIP().toString();
    object[F("softAPmacAddress")] = WiFi.softAPmacAddress();
    return esp8266utils::toString(object);
  }

 private:
  
  WiFiEventHandler _softAPModeStationConnectedHandler;
  WiFiEventHandler _softAPModeStationDisconnectedHandler;
  WiFiEventHandler _softAPModeProbeRequestReceivedHandler;

  String macAddress(const unsigned char *mac) {
    
    char buffer[20];
    snprintf(buffer, sizeof(buffer), "%02x:%02x:%02x:%02x:%02x:%02x", mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
    return String(buffer);
  }
};
}  // namespace esp8266utils

#if !defined(NO_GLOBAL_INSTANCES)
extern esp8266utils::WiFiAPConfig WIFI_AP_CFG;
#endif  // NO_GLOBAL_INSTANCES