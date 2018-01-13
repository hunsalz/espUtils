#include "WiFiAPService.h"

namespace esp8266util {

WiFiAPService::~WiFiAPService() { end(); }

bool WiFiAPService::available() { return WiFi.status(); }

bool WiFiAPService::begin(const char *ssid, const char *passphrase, int channel, int ssid_hidden, int max_connection, bool autoConnect, bool persistent) {
  
  WiFi.enableAP(true);
  WiFi.setAutoConnect(true);
  WiFi.persistent(false);
  WiFi.softAPdisconnect();
  // verbose callback handlers
  // _softAPModeStationConnectedHandler = WiFi.onSoftAPModeStationConnected([this](const WiFiEventSoftAPModeStationConnected&
  // event) {
  //   LOG.verbose(F("MAC address [%s] joined AP."), macAddress(event.mac).c_str());
  // });
  // _softAPModeStationDisconnectedHandler = WiFi.onSoftAPModeStationDisconnected([this](const
  // WiFiEventSoftAPModeStationDisconnected& event) {
  //   LOG.verbose(F("MAC address [%s] disappeared from AP."), macAddress(event.mac).c_str());
  // });
  // _softAPModeProbeRequestReceivedHandler = WiFi.onSoftAPModeProbeRequestReceived([this](const
  // WiFiEventSoftAPModeProbeRequestReceived& event) {
  //   LOG.verbose(F("RSSI is [%d]"), event.rssi);
  // });
  // enable AP mode
  if (WiFi.softAP(ssid, passphrase, channel, ssid_hidden, max_connection)) {
    // reflect change to MDNS
    if (MDNS_SERVICE.available()) {
      MDNS_SERVICE.getMDNSResponder().update();
    }
    LOG.verbose(F("Soft AP established successful. IP address of AP is: %s"), WiFi.softAPIP().toString().c_str());
  }

  return available();
}

bool WiFiAPService::end() {
  
  WiFi.softAPdisconnect();

  return available();
}

ESP8266WiFiClass &WiFiAPService::getWiFi() { return WiFi; }

JsonObject &WiFiAPService::getDetails() {
  
  DynamicJsonBuffer jsonBuffer;
  JsonObject &json = jsonBuffer.createObject();
  json[F("softAPgetStationNum")] = WiFi.softAPgetStationNum();
  json[F("softAPIP")] = WiFi.softAPIP().toString();
  json[F("softAPmacAddress")] = WiFi.softAPmacAddress();

  return json;
}

String WiFiAPService::macAddress(const unsigned char *mac) {
  
  char buffer[20];
  snprintf(buffer, sizeof(buffer), "%02x:%02x:%02x:%02x:%02x:%02x", mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);

  return String(buffer);
}

extern WiFiAPService WIFI_STATION = WiFiAPService();
} // namespace esp8266util
