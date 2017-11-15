#include "WiFiService.h"

namespace esp8266util {

  WiFiService::~WiFiService() {
    end();
  }

  bool WiFiService::available() {
    return WiFi.isConnected();
  }

  bool WiFiService::begin(uint8_t retries, bool autoConnect, bool persistent) {

    if (!available()) {
      // general settings
      WiFi.enableSTA(true);
      WiFi.setAutoConnect(true);
      WiFi.persistent(false);
      
      // try to connect
      Log.verbose("Trying to connect to WiFi ");
      while (_wifiMulti.run() != WL_CONNECTED && retries-- > 0) { // try to connect for given amount of retries
        Serial.print(F("."));
        delay(300);
      }
      Serial.println();
      // log WiFi connection result
      if (retries > 0) {
        // reflect change to MDNS
        if (MDNS_SERVICE.available()) {
          MDNS_SERVICE.getMDNSResponder().update();
        }
        Log.notice(F("WiFi successful connected with IP: %s" CR), WiFi.localIP().toString().c_str());

        //WiFi.onEvent([](WiFiEvent_t e) {
          //Serial.printf("Event wifi -----> %d\n", e);
        //});

        // TODO extend verbose logs
        _stationModeConnectedHandler = WiFi.onStationModeConnected([this](const WiFiEventStationModeConnected& event) {
          Log.verbose(F("Connected with WiFi station." CR));
        });
        _stationModeDisconnectedHandler = WiFi.onStationModeDisconnected([this](const WiFiEventStationModeDisconnected& event) {
          Log.verbose(F("Disconnected from WiFi station." CR));
        });
        _stationModeAuthModeChangedHandler = WiFi.onStationModeAuthModeChanged([this](const WiFiEventStationModeAuthModeChanged& event) {
          Log.verbose(F("WiFi authentication mode changed." CR));
        });
        _stationModeGotIPHandler = WiFi.onStationModeGotIP([this](const WiFiEventStationModeGotIP& event) {
          Log.verbose(F("Received IP from WiFi station." CR));
        });
        _stationModeDHCPTimeoutHandler = WiFi.onStationModeDHCPTimeout([this]() {
          Log.verbose(F("Got DHCP timeout from WiFi station." CR));
        });

      } else {
        Log.error(F("Failed to setup a WiFi connection. Please check your WiFi availability / accessibility and retry." CR));
      }
    }

    return available();
  }

  bool WiFiService::end() {

    WiFi.disconnect();

    return available();
  }

  ESP8266WiFiClass& WiFiService::getWiFi() {
    return WiFi;
  }

  ESP8266WiFiMulti& WiFiService::getWiFiMulti() {
    return _wifiMulti;
  }

  JsonObject& WiFiService::getDetails() {

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
}
