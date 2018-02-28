#include "WiFiAPConfig.h"

namespace esp8266util {

bool WiFiAPConfig::begin(const char *ssid, const char *passphrase, int channel, int ssid_hidden, int max_connection,
                         bool autoConnect, bool persistent) {

  WiFi.enableAP(true);
  WiFi.setAutoConnect(true);
  WiFi.persistent(false);
  WiFi.softAPdisconnect();
  // add default verbose callback handlers if no handler is set
  if (!_softAPModeStationConnectedHandler) {
    onSoftAPModeStationConnected([&](const WiFiEventSoftAPModeStationConnected &event) {
      LOG.verbose(F("MAC address [%s] joined AP."), macAddress(event.mac).c_str());
    });
  }
  if (!_softAPModeStationDisconnectedHandler) {
    onSoftAPModeStationDisconnected([&](const WiFiEventSoftAPModeStationDisconnected &event) {
      LOG.verbose(F("MAC address [%s] disappeared from AP."), macAddress(event.mac).c_str());
    });
  }
  if (!_softAPModeProbeRequestReceivedHandler) {
    onSoftAPModeProbeRequestReceived([&](const WiFiEventSoftAPModeProbeRequestReceived &event) {
      LOG.trace(F("RSSI is [%d]"), event.rssi);
    });
  }
  // enable AP
  if (WiFi.softAP(ssid, passphrase, channel, ssid_hidden, max_connection)) {
    LOG.verbose(F("Soft AP established successful. IP address of AP is: %s"), WiFi.softAPIP().toString().c_str());
  }

  return WiFi.isConnected();
}

void WiFiAPConfig::end() { WiFi.softAPdisconnect(); }

WiFiEventHandler WiFiAPConfig::onSoftAPModeStationConnected(std::function<void(const WiFiEventSoftAPModeStationConnected&)> f) {

  _softAPModeStationConnectedHandler = WiFi.onSoftAPModeStationConnected(f);

  return _softAPModeStationConnectedHandler;
}

WiFiEventHandler WiFiAPConfig::onSoftAPModeStationDisconnected(std::function<void(const WiFiEventSoftAPModeStationDisconnected&)> f) {

  _softAPModeStationDisconnectedHandler = WiFi.onSoftAPModeStationDisconnected(f);

  return _softAPModeStationDisconnectedHandler;
}

WiFiEventHandler WiFiAPConfig::onSoftAPModeProbeRequestReceived(std::function<void(const WiFiEventSoftAPModeProbeRequestReceived&)> f) {

  _softAPModeProbeRequestReceivedHandler = WiFi.onSoftAPModeProbeRequestReceived(f);

  return _softAPModeProbeRequestReceivedHandler;
}

const char* WiFiAPConfig::getConfigAsJson() {

  DynamicJsonBuffer jsonBuffer;
  JsonObject &json = jsonBuffer.createObject();
  // TODO

  return esp8266util::toString(json);
}

const char* WiFiAPConfig::getDetails() {

  DynamicJsonBuffer jsonBuffer;
  JsonObject &json = jsonBuffer.createObject();
  json[F("softAPgetStationNum")] = WiFi.softAPgetStationNum();
  json[F("softAPIP")] = WiFi.softAPIP().toString();
  json[F("softAPmacAddress")] = WiFi.softAPmacAddress();

  return esp8266util::toString(json);
}

String WiFiAPConfig::macAddress(const unsigned char *mac) {

  char buffer[20];
  snprintf(buffer, sizeof(buffer), "%02x:%02x:%02x:%02x:%02x:%02x", mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);

  return String(buffer);
}
} // namespace esp8266util

#if !defined(NO_GLOBAL_INSTANCES)
esp8266util::WiFiAPConfig WIFI_AP_CFG;
#endif // NO_GLOBAL_INSTANCES