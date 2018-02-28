#include "WiFiStaConfig.h"

namespace esp8266utils {

bool WiFiStaConfig::begin(uint8_t retries, bool autoConnect, bool persistent) {

  // TODO
  bool line_feed = true;
  uint8_t i = retries;
  // general settings
  WiFi.enableSTA(true);
  WiFi.setAutoConnect(true);
  WiFi.persistent(false);
  // add default verbose callback handlers if no handler is set
  if (!_stationModeConnectedHandler) {
    onStationModeConnected([&](const WiFiEventStationModeConnected &event) {
      if (line_feed) {
        line_feed = false;
        Serial.println();
      }
      LOG.verbose(F("Connected with WiFi station after [%d] attempts"), retries - i);
    });
  }
  if (!_stationModeDisconnectedHandler) {
    onStationModeDisconnected([&](const WiFiEventStationModeDisconnected &event) {
      if (line_feed) {
        line_feed = false;
        Serial.println();
      }
      LOG.verbose(F("WiFi connection [%s] dropped. Reason: %d"), event.ssid.c_str(), event.reason);
    });
  }
  if (!_stationModeAuthModeChangedHandler) {
    onStationModeAuthModeChanged([&](const WiFiEventStationModeAuthModeChanged &event) {
      if (line_feed) {
        line_feed = false;
        Serial.println();
      }
      LOG.verbose(F("WiFi authentication mode changed."));
    });
  }
  if (!_stationModeGotIPHandler) {
    onStationModeGotIP([&](const WiFiEventStationModeGotIP &event) {
      if (line_feed) {
        line_feed = false;
        Serial.println();
      }
      LOG.verbose(F("Received IP [%s] from WiFi station."), event.ip.toString().c_str());
    });
  }
  if (!_stationModeDHCPTimeoutHandler) {
    onStationModeDHCPTimeout([&]() {
      if (line_feed) {
        line_feed = false;
        Serial.println();
      }
      LOG.verbose(F("Received DHCP timeout from WiFi station."));
    });
  }
  // try to connect
  LOG.verbose(F("Connecting to WiFi network"));
  while (_wifiMulti.run() != WL_CONNECTED && i-- > 0) { // try to connect for given amount of retries
    Serial.print(F("."));
    delay(300);
  }
  if (line_feed) {
    line_feed = false;
    Serial.println();
  }
  if (retries <= 0) {
    LOG.error(F("Couldn't establish any WiFi connection."));
  }
  // // log WiFi connection result
  // if (i > 0) {
  //   // reflect change to MDNS
  //   if (MDNS_SERVICE.available()) {
  //     MDNS_SERVICE.getMDNSResponder().update();
  //   }
  //   LOG.verbose(F("WiFi successful connected with IP: %s"), WiFi.localIP().toString().c_str());
  // } else {
  //   LOG.error(F("Coudn't establish any WiFi connection."));
  // }

  return WiFi.status();
}

void end() {
  // TODO
  //WiFi.stopAll();
}

bool WiFiStaConfig::addAP(const char *ssid, const char *passphrase) { return _wifiMulti.addAP(ssid, passphrase); }

WiFiEventHandler WiFiStaConfig::onStationModeConnected(std::function<void(const WiFiEventStationModeConnected &)> f) {

  _stationModeConnectedHandler = WiFi.onStationModeConnected(f);

  return _stationModeConnectedHandler;
}

WiFiEventHandler WiFiStaConfig::onStationModeDisconnected(std::function<void(const WiFiEventStationModeDisconnected &)> f) {

  _stationModeDisconnectedHandler = WiFi.onStationModeDisconnected(f);

  return _stationModeDisconnectedHandler;
}

WiFiEventHandler WiFiStaConfig::onStationModeAuthModeChanged(std::function<void(const WiFiEventStationModeAuthModeChanged &)> f) {

  _stationModeAuthModeChangedHandler = WiFi.onStationModeAuthModeChanged(f);

  return _stationModeAuthModeChangedHandler;
}

WiFiEventHandler WiFiStaConfig::onStationModeGotIP(std::function<void(const WiFiEventStationModeGotIP &)> f) {

  _stationModeGotIPHandler = WiFi.onStationModeGotIP(f);

  return _stationModeGotIPHandler;
}

WiFiEventHandler WiFiStaConfig::onStationModeDHCPTimeout(std::function<void(void)> f) {

  _stationModeDHCPTimeoutHandler = WiFi.onStationModeDHCPTimeout(f);

  return _stationModeDHCPTimeoutHandler;
}

const char* WiFiStaConfig::getConfigAsJson() {

  DynamicJsonBuffer jsonBuffer;
  JsonObject &json = jsonBuffer.createObject();
  // TODO

  return esp8266utils::toString(json);
}

const char* WiFiStaConfig::getDetails() {

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

  return esp8266utils::toString(json);
}
} // namespace esp8266utils

#if !defined(NO_GLOBAL_INSTANCES)
esp8266utils::WiFiStaConfig WIFI_STA_CFG;
#endif // NO_GLOBAL_INSTANCES