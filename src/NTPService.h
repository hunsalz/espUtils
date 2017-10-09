#pragma once

#include <ArduinoLog.h> // https://github.com/thijse/Arduino-Log
#include <NTPClient.h> // https://github.com/arduino-libraries/NTPClient
#include <WiFiUdp.h> // https://github.com/esp8266/Arduino/tree/master/libraries/ESP8266WiFi

#include "Service.h"

namespace esp8266util {

	class NTPService : public Service {

		public:

	    NTPService();

	    ~NTPService();

	    bool isRunning();

	    bool start();

	    bool stop();

	    NTPClient getNTPClient();

	  private:

	    WiFiUDP ntpUDP;
	    NTPClient ntpClient;

	    bool running = false;
	};
}
