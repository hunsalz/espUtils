#pragma once

#include <Arduino.h>          // https://github.com/esp8266/Arduino/blob/master/cores/esp8266/Arduino.h
#include <ArduinoJson.h>      // https://github.com/bblanchon/ArduinoJson
#include <ESP8266WiFi.h>      // https://github.com/esp8266/Arduino/blob/master/libraries/ESP8266WiFi/src/ESP8266WiFi.h
#include <ESP8266WiFiMulti.h> // https://github.com/esp8266/Arduino/tree/master/libraries/ESP8266WiFi/src/ESP8266WiFiMulti.h

#include "Logging.hpp"

namespace esp8266utils {

inline bool setupWiFiSta(ESP8266WiFiMulti& wifiMulti, uint8_t retries = 32, bool autoConnect = true, bool persistent = false) {

  // general settings
  WiFi.mode(WIFI_STA);
  WiFi.enableSTA(true);
  WiFi.setAutoConnect(autoConnect);
  WiFi.persistent(persistent);
  // try to connect for given amount of retries
  while (wifiMulti.run() != WL_CONNECTED && retries-- > 0) { 
    #ifdef DEBUG_ESP_PORT
    DEBUG_ESP_PORT.print(F("."));
    #endif
    delay(300);
  }
  #ifdef DEBUG_ESP_PORT
  DEBUG_ESP_PORT.println();
  #endif
  // output WiFi status
  if (retries <= 0) {
    ERROR_FP(F("Couldn't establish any WiFi connection."));
  } else {
    VERBOSE_FP(F("WiFi successful connected with IP: %s"), WiFi.localIP().toString().c_str());
  }

  return WiFi.status();
}

inline size_t serializeWiFiSta(String& output) {

  DynamicJsonDocument doc;
  JsonObject object = doc.to<JsonObject>();
  object[F("isConnected")] = WiFi.isConnected();
  object[F("autoConnect")] = WiFi.getAutoConnect();
  object[F("localIP")] = WiFi.localIP().toString();
  object[F("macAddress")] = WiFi.macAddress();
  object[F("subnetMask")] = WiFi.subnetMask().toString();
  object[F("gatewayIP")] = WiFi.gatewayIP().toString();
  object[F("dnsIP")] = WiFi.dnsIP().toString();
  object[F("hostname")] = WiFi.hostname();
  object[F("status")] = WiFi.status();
  object[F("ssid")] = WiFi.SSID();
  object[F("psk")] = WiFi.psk();
  object[F("bssId")] = WiFi.BSSIDstr();
  object[F("rssi")] = WiFi.RSSI();
  object[F("channel")] = WiFi.channel();
  object[F("sleepMode")] = WiFi.getSleepMode();
  object[F("phyMode")] = WiFi.getPhyMode();
  object[F("wiFiMode")] = WiFi.getMode();
  serializeJson(object, output);
  return measureJson(object);
}

} // namespace esp8266utils