#pragma once

#include <ArduinoLog.h> // https://github.com/thijse/Arduino-Log
#include <ArduinoJson.h> // https://github.com/bblanchon/ArduinoJson
#include <NTPClient.h> // https://github.com/arduino-libraries/NTPClient
#include <WiFiUdp.h> // https://github.com/esp8266/Arduino/tree/master/libraries/ESP8266WiFi

#include "Service.h"

namespace esp8266util {

	class NTPService : public Service {

		public:

	    NTPService(const char* poolServerName, int timeOffset = 0, int updateInterval = 60000);

	    ~NTPService();

	    bool isRunning();

	    bool start();

	    bool stop();

	    NTPClient getNTPClient();

			const char* getPoolServerName();
			int getTimeOffset();
			int getUpdateInterval();

			JsonObject& getDetails();

	  private:

	    WiFiUDP ntpUDP;
	    NTPClient ntpClient;

			const char* poolServerName;
			int timeOffset;
			int updateInterval;

	    bool running = false;
	};
}
