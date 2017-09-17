#include "WiFiStaService.h"

WiFiStaService::WiFiStaService() {
}

WiFiStaService::~WiFiStaService() {
  stop();
}

bool WiFiStaService::isRunning() {
  return WiFi.isConnected();
}

bool WiFiStaService::start() {

  uint8_t retries = 20;
  if (!isRunning()) {
    // general settings
    WiFi.enableSTA(true);
    WiFi.persistent(false);
    WiFi.setAutoConnect(true);

    /**
      connectedEventHandler = WiFi.onStationModeConnected([](const WiFiEventStationModeConnected& event) {
        Log.verbose(F("Station connected"));
      });
      disconnectedEventHandler = WiFi.onStationModeDisconnected([](const WiFiEventStationModeDisconnected& event) {
        Log.verbose(F("Station disconnected"));
      });
      authModeChangedEventHandler = WiFi.onStationModeAuthModeChanged([](const WiFiEventStationModeAuthModeChanged & event) {
        Log.verbose(F("Station auth mode changed"));
      });
      gotIpEventHandler = WiFi.onStationModeGotIP([](const WiFiEventStationModeGotIP& event) {
        Log.verbose(F("Station connected, IP: %s" CR), WiFi.localIP().toString().c_str());
      });
    */

    // try to connect
    Log.verbose("Trying to connect WiFi ");
    while (wifiMulti.run() != WL_CONNECTED && retries-- > 0) { // try to connect at least 20 times
      // TODO Log.verbose prints "V: .V: .V: .V: .V: .V:" instead of "....."
      Log.verbose(F("."));
      delay(300);
    }
    Serial.println();
  }
  if (retries == 0) {
    Log.error(F("Failed to setup a WiFi connection. Please check your WiFi availability / accessibility and restart." CR));
    stop(); // => stop service
  } else {
    Log.notice(F("WiFi successful connected. IP address is: %s" CR), WiFi.localIP().toString().c_str());
  }

  return isRunning();
}

bool WiFiStaService::stop() {

  if (isRunning()) {
    WiFi.disconnect();
  }

  return isRunning();
}

ESP8266WiFiClass* WiFiStaService::getWiFi() {
  return &WiFi;
}

ESP8266WiFiMulti* WiFiStaService::getWiFiMulti() {
  return &wifiMulti;
}

JsonObject& WiFiStaService::getWiFiDetails() {

  DynamicJsonBuffer jsonBuffer;
  JsonObject &json = jsonBuffer.createObject();
  json[F("isConnected")] = getWiFi()->isConnected();
  json[F("autoConnect")] = getWiFi()->getAutoConnect();
  json[F("localIP")] = getWiFi()->localIP().toString();
  json[F("macAddress")] = getWiFi()->macAddress();
  json[F("subnetMask")] = getWiFi()->subnetMask().toString();
  json[F("gatewayIP")] = getWiFi()->gatewayIP().toString();
  json[F("dnsIP")] = getWiFi()->dnsIP().toString();
  json[F("hostname")] = getWiFi()->hostname();
  json[F("status")] = getWiFi()->status();
  json[F("ssid")] = getWiFi()->SSID();
  json[F("psk")] = getWiFi()->psk();
  json[F("bssId")] = getWiFi()->BSSIDstr();
  json[F("rssi")] = getWiFi()->RSSI();
  json[F("channel")] = getWiFi()->channel();
  json[F("sleepMode")] = getWiFi()->getSleepMode();
  json[F("phyMode")] = getWiFi()->getPhyMode();
  json[F("wiFiMode")] = getWiFi()->getMode();

  return json;
}
