#ifndef WIFIAPSERVICE_H
#define WIFIAPSERVICE_H

#ifdef ESP8266
extern "C" {
  #include "user_interface.h" // Expressif ESP8266 Non-OS API
}
#endif

#include <ArduinoLog.h> // https://github.com/thijse/Arduino-Log
#include <ESP8266WiFi.h> // https://github.com/esp8266/Arduino/blob/master/libraries/ESP8266WiFi/src/ESP8266WiFi.h
#include <ArduinoJson.h> // https://github.com/bblanchon/ArduinoJson

#include "Service.h"

namespace esp8266util {

  class WiFiAPService : public Service {

    public:

      ~WiFiAPService();

      bool isSetup();

      bool available();

      bool start();

      bool stop();

      ESP8266WiFiClass& getWiFi();

      bool setup(const char* ssid, const char* passphrase, int channel = 1, int ssid_hidden = 0, int max_connection = 5, bool autoConnect = true, bool persistent = false);

      JsonObject& getDetails();

      String macAddress(const unsigned char* mac);

    private:

      const char* _ssid;
      const char* _passphrase;
      int _channel = 1;
      int _ssid_hidden = 0;
      int _max_connection = 4;

      WiFiEventHandler _softAPModeStationConnectedHandler;
      WiFiEventHandler _softAPModeStationDisconnectedHandler;
      WiFiEventHandler _softAPModeProbeRequestReceivedHandler;

      bool _setupDone = false;
  };
}

#endif // WIFIAPSERVICE_H
