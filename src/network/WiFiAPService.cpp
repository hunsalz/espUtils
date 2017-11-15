#include "WiFiAPService.h"

namespace esp8266util {

  WiFiAPService::~WiFiAPService() {
    stop();
  }

  bool WiFiAPService::isSetup() {
    return _setupDone;
  }

  bool WiFiAPService::isRunning() {
    return WiFi.status();
  }

  bool WiFiAPService::start() {

    if (isSetup()) {
      WiFi.softAPdisconnect();
      // TODO reflect changes to MDNS
      if (WiFi.softAP(_ssid, _passphrase, _channel, _ssid_hidden, _max_connection)) {
        Log.notice(F("Soft AP established successful. IP address of AP is: %s" CR), WiFi.softAPIP().toString().c_str());
        
        _softAPModeStationConnectedHandler = WiFi.onSoftAPModeStationConnected([this](const WiFiEventSoftAPModeStationConnected& event) {
          Log.verbose(F("MAC address [%s] joined AP." CR), macAddress(event.mac).c_str());
        });
        _softAPModeStationDisconnectedHandler = WiFi.onSoftAPModeStationDisconnected([this](const WiFiEventSoftAPModeStationDisconnected& event) {
          Log.verbose(F("MAC address [%s] disappeared from AP." CR), macAddress(event.mac).c_str());
        });
        _softAPModeProbeRequestReceivedHandler = WiFi.onSoftAPModeProbeRequestReceived([this](const WiFiEventSoftAPModeProbeRequestReceived& event) {
          //Log.verbose(F("RSSI is [%d]" CR), event.rssi);
        });
      } else {
        Log.error(F("Couldn't establish a soft access point." CR));
      }
    } else {
      Log.error("Call setup() first.");
    }

    return isRunning();
  }

  bool WiFiAPService::stop() {

    WiFi.softAPdisconnect();

    return isRunning();
  }

  ESP8266WiFiClass& WiFiAPService::getWiFi() {
    return WiFi;
  }

  bool WiFiAPService::setup(const char* ssid, const char* passphrase, int channel, int ssid_hidden, int max_connection, bool autoConnect, bool persistent) {

    _ssid = ssid;
    _passphrase = passphrase;
    _channel = channel;
    _ssid_hidden = ssid_hidden;
    _max_connection = max_connection;
    // general settings
    WiFi.enableAP(true);
    WiFi.setAutoConnect(true);
    WiFi.persistent(false);

    _setupDone = true;

    return isSetup();
  }

  JsonObject& WiFiAPService::getDetails() {

    DynamicJsonBuffer jsonBuffer;
    JsonObject &json = jsonBuffer.createObject();
    json[F("softAPgetStationNum")] = WiFi.softAPgetStationNum();
    json[F("softAPIP")] = WiFi.softAPIP().toString();
    json[F("softAPmacAddress")] = WiFi.softAPmacAddress();

    return json;
  }

  String WiFiAPService::macAddress(const unsigned char* mac) {
    
    char buffer[20];
    snprintf(buffer, sizeof(buffer), "%02x:%02x:%02x:%02x:%02x:%02x", mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
    
    return String(buffer  );
  }
}
