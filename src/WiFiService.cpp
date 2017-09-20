#include "WiFiService.h"

WiFiService::WiFiService() {
}

WiFiService::~WiFiService() {
  stop();
}

bool WiFiService::start() {

  if (retries > 0) {
    if (!isRunning()) {
      // try to connect
      Log.verbose("Trying to connect WiFi ");
      while (wifiMulti.run() != WL_CONNECTED && retries-- > 0) { // try to connect for given amount of retries
        Serial.print(F("."));
        delay(300);
      }
      Serial.println();
      // log WiFi connection result
      if (retries > 0) {
        Log.notice(F("WiFi successful connected to IP: %s" CR), WiFi.localIP().toString().c_str());
      } else {
        Log.error(F("Failed to setup a WiFi connection. Please check your WiFi availability / accessibility and retry." CR));
      }
    }
  } else {
    Log.error("Please provide appropriate wiFi settings.");
  }

  return isRunning();
}

bool WiFiService::stop() {

  WiFi.disconnect();

  return isRunning();
}

bool WiFiService::isRunning() {
  return WiFi.isConnected();
}

bool WiFiService::addAP(const char* ssid, const char *passphrase) {
  return getWiFiMulti()->addAP(ssid, passphrase);
}

ESP8266WiFiClass* WiFiService::getWiFi() {
  return &WiFi;
}

ESP8266WiFiMulti* WiFiService::getWiFiMulti() {
  return &wifiMulti;
}

void WiFiService::setupWiFi(uint8_t retries, bool autoConnect, bool persistent) {

  this->retries = retries;
  // general settings
  WiFi.enableSTA(true);
  WiFi.setAutoConnect(true);
  WiFi.persistent(false);
}

JsonObject& WiFiService::getDetails() {

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
