#pragma once

#ifdef ESP8266
extern "C" {
  #include "user_interface.h" // Expressif ESP8266 Non-OS API
}
#endif

#include <ArduinoLog.h> // https://github.com/thijse/Arduino-Log
#include <ESP8266WiFi.h> // https://github.com/esp8266/Arduino/blob/master/libraries/ESP8266WiFi/src/ESP8266WiFi.h
#include <ESP8266mDNS.h> // https://github.com/esp8266/Arduino/blob/master/libraries/ESP8266mDNS/ESP8266mDNS.h
#include <ArduinoJson.h> // https://github.com/bblanchon/ArduinoJson

#include "Service.h"

namespace esp8266util {

	class WiFiAPService : public Service {

		public:

	    ~WiFiAPService();

			bool isSetup();

	    bool isRunning();

	    bool start();

	    bool stop();

			ESP8266WiFiClass *getWiFi();

			bool setup(const char* ssid, const char* passphrase, int channel = 1, int ssid_hidden = 0, int max_connection = 5, bool autoConnect = true, bool persistent = false);

			// TODO refactor MDNS (separate as single service?)
			bool enableMDNS(const char* hostName, unsigned int port = 80, uint32_t ttl = 120);

			JsonObject& getDetails();

		private:

			const char* ssid;
			const char* passphrase;
			int channel  = 1;
			int ssid_hidden = 0;
			int max_connection = 4;

			bool setupDone = false;
	};
}
