#include "WiFiAPService.h"

namespace esp8266util {

  WiFiAPService::WiFiAPService() {
  }

  WiFiAPService::~WiFiAPService() {
    stop();
  }

  bool WiFiAPService::isRunning() {
    return WiFi.status();
  }

  bool WiFiAPService::start() {

    WiFi.softAPdisconnect();
    // TODO MDNS update
    if (WiFi.softAP(ssid, passphrase, channel, ssid_hidden, max_connection)) {
      Log.notice(F("Soft AP established successful. IP address of AP is: %s" CR), WiFi.softAPIP().toString().c_str());
    } else {
      Log.error(F("Couldn't establish a soft access point." CR));
    }

    return isRunning();
  }

  bool WiFiAPService::stop() {

    WiFi.softAPdisconnect();

    return isRunning();
  }

  ESP8266WiFiClass* WiFiAPService::getWiFi() {
    return &WiFi;
  }

  void WiFiAPService::setup(const char* ssid, const char* passphrase, int channel, int ssid_hidden, int max_connection, bool autoConnect, bool persistent) {

    this->ssid = ssid;
    this->passphrase = passphrase;
    this->channel = channel;
    this->ssid_hidden = ssid_hidden;
    this->max_connection = max_connection;
    // general settings
    WiFi.enableAP(true);
    WiFi.setAutoConnect(true);
    WiFi.persistent(false);
  }

  bool WiFiAPService::enableMDNS(const char* hostName, unsigned int port, uint32_t ttl) {

    bool succeeded = false;
    // add <domain name>.local (mDNS)
    if (MDNS.begin(hostName, port, ttl)) {
      // add service
      MDNS.addService("http", "tcp", port);
      succeeded = true;
    }
    if (succeeded) {
      Log.notice(F("MDNS enabled to http://%s.local" CR), hostName);
    } else {
      Log.error(F("MDNS failed for http://%s.local" CR), hostName);
    }

    return succeeded;
  }

  JsonObject& WiFiAPService::getDetails() {

    DynamicJsonBuffer jsonBuffer;
    JsonObject &json = jsonBuffer.createObject();
    json[F("softAPgetStationNum")] = getWiFi()->softAPgetStationNum();
    json[F("softAPIP")] = getWiFi()->softAPIP().toString();
    json[F("softAPmacAddress")] = getWiFi()->softAPmacAddress();

    return json;
  }
}
