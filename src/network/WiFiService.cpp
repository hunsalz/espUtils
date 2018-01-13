#include "WiFiService.h"

namespace esp8266util {

WiFiService::~WiFiService() { end(); }

bool WiFiService::available() { return WiFi.isConnected(); }

bool WiFiService::begin(uint8_t retries, bool autoConnect, bool persistent) {
  
  uint8_t i = retries;
  // general settings
  WiFi.enableSTA(true);
  WiFi.setAutoConnect(true);
  WiFi.persistent(false);
  // default callback handlers
  _stationModeConnectedHandler = WiFi.onStationModeConnected([&](const WiFiEventStationModeConnected &event) {
    LOG.verbose(F("Connected with WiFi station after [%d] attempts"), retries - i);
  });
  _stationModeDisconnectedHandler = WiFi.onStationModeDisconnected([](const WiFiEventStationModeDisconnected &event) {
    LOG.verbose(F("WiFi connection [%s] dropped. Reason: %d"), event.ssid.c_str(), event.reason);
  });
  _stationModeAuthModeChangedHandler = WiFi.onStationModeAuthModeChanged(
      [](const WiFiEventStationModeAuthModeChanged &event) { LOG.verbose(F("WiFi authentication mode changed.")); });
  _stationModeGotIPHandler = WiFi.onStationModeGotIP([](const WiFiEventStationModeGotIP &event) {
    LOG.verbose(F("Received IP [%s] from WiFi station."), event.ip.toString().c_str());
  });
  _stationModeDHCPTimeoutHandler =
      WiFi.onStationModeDHCPTimeout([]() { LOG.verbose(F("Received DHCP timeout from WiFi station.")); });
  // try to connect
  LOG.verbose("Trying to connect to WiFi ");
  while (_wifiMulti.run() != WL_CONNECTED && i-- > 0) { // try to connect for given amount of retries
    Serial.print(F("."));
    delay(300);
  }
  Serial.println();
  // log WiFi connection result
  if (i > 0) {
    // reflect change to MDNS
    if (MDNS_SERVICE.available()) {
      MDNS_SERVICE.getMDNSResponder().update();
    }
    LOG.verbose(F("WiFi successful connected with IP: %s"), WiFi.localIP().toString().c_str());
  }

  return available();
}

bool WiFiService::end() {
  
  WiFi.disconnect();

  return available();
}

ESP8266WiFiClass &WiFiService::getWiFi() { return WiFi; }

ESP8266WiFiMulti &WiFiService::getWiFiMulti() { return _wifiMulti; }

JsonObject &WiFiService::getDetails() {
  
  DynamicJsonBuffer jsonBuffer;
  JsonObject &json = jsonBuffer.createObject();
  json[F("isConnected")] = WiFi.isConnected();
  json[F("autoConnect")] = WiFi.getAutoConnect();
  json[F("localIP")] = WiFi.localIP().toString();
  json[F("macAddress")] = WiFi.macAddress();
  json[F("subnetMask")] = WiFi.subnetMask().toString();
  json[F("gatewayIP")] = WiFi.gatewayIP().toString();
  json[F("dnsIP")] = WiFi.dnsIP().toString();
  json[F("hostname")] = WiFi.hostname();
  json[F("status")] = WiFi.status();
  json[F("ssid")] = WiFi.SSID();
  json[F("psk")] = WiFi.psk();
  json[F("bssId")] = WiFi.BSSIDstr();
  json[F("rssi")] = WiFi.RSSI();
  json[F("channel")] = WiFi.channel();
  json[F("sleepMode")] = WiFi.getSleepMode();
  json[F("phyMode")] = WiFi.getPhyMode();
  json[F("wiFiMode")] = WiFi.getMode();

  return json;
}

extern WiFiService WIFI_CLIENT = WiFiService();
} // namespace esp8266util
