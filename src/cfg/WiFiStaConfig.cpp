#include "WiFiStaConfig.h"

namespace esp8266utils {

bool WiFiStaConfig::begin(uint8_t retries, bool autoConnect, bool persistent) {

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
      VERBOSE_MSG_P(F("Connected with WiFi station after [%d] attempts"), retries - i);
    });
  }
  if (!_stationModeDisconnectedHandler) {
    onStationModeDisconnected([&](const WiFiEventStationModeDisconnected &event) {
      if (line_feed) {
        line_feed = false;
        Serial.println();
      }
      VERBOSE_MSG_P(F("WiFi connection [%s] dropped. Reason: %d"), event.ssid.c_str(), event.reason);
    });
  }
  if (!_stationModeAuthModeChangedHandler) {
    onStationModeAuthModeChanged([&](const WiFiEventStationModeAuthModeChanged &event) {
      if (line_feed) {
        line_feed = false;
        Serial.println();
      }
      VERBOSE_MSG_P(F("WiFi authentication mode changed."));
    });
  }
  if (!_stationModeGotIPHandler) {
    onStationModeGotIP([&](const WiFiEventStationModeGotIP &event) {
      if (line_feed) {
        line_feed = false;
        Serial.println();
      }
      VERBOSE_MSG_P(F("Received IP [%s] from WiFi station."), event.ip.toString().c_str());
    });
  }
  if (!_stationModeDHCPTimeoutHandler) {
    onStationModeDHCPTimeout([&]() {
      if (line_feed) {
        line_feed = false;
        Serial.println();
      }
      VERBOSE_MSG_P(F("Received DHCP timeout from WiFi station."));
    });
  }
  // try to connect
  VERBOSE_MSG_P(F("Connecting to WiFi network"));
  while (_wifiMulti.run() != WL_CONNECTED && i-- > 0) { // try to connect for given amount of retries
    Serial.print(F("."));
    delay(300);
  }
  if (line_feed) {
    line_feed = false;
    Serial.println();
  }
  if (retries <= 0) {
    ERROR_MSG_P(F("Couldn't establish any WiFi connection."));
  }
  // // log WiFi connection result
  // if (i > 0) {
  //   // reflect change to MDNS
  //   if (MDNS_SERVICE.available()) {
  //     MDNS_SERVICE.getMDNSResponder().update();
  //   }
  //   VERBOSE_MSG_P(F("WiFi successful connected with IP: %s"), WiFi.localIP().toString().c_str());
  // } else {
  //   ERROR_MSG_P(F("Coudn't establish any WiFi connection."));
  // }

  return WiFi.status();
}

void end() {
  // TODO
  // WiFi.stopAll();
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

String WiFiStaConfig::getDetails() {

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

  return esp8266utils::toString(object);
}
} // namespace esp8266utils

#if !defined(NO_GLOBAL_INSTANCES)
esp8266utils::WiFiStaConfig WIFI_STA_CFG;
#endif // NO_GLOBAL_INSTANCES