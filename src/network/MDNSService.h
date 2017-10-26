#pragma once

#include <ArduinoLog.h> // https://github.com/thijse/Arduino-Log
#include <ESP8266mDNS.h> // https://github.com/esp8266/Arduino/blob/master/libraries/ESP8266mDNS/ESP8266mDNS.h
#include <ArduinoJson.h> // https://github.com/bblanchon/ArduinoJson

#include "Service.h"

namespace esp8266util {

	class MDNSService : public Service {

		public:

	    ~MDNSService();

			bool isSetup();

	    bool isRunning();

	    bool start();

	    bool stop();

			MDNSResponder* getMDNSResponder();

			bool setup(const char* domainName);

			const char* getDomainName();

			JsonObject& getDetails();

	  private:

			const char* domainName;
			uint16_t port;
			uint32_t ttl;

			bool setupDone = false;
			bool running = false;
	};
}
