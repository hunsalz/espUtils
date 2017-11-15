#ifndef MDNSSERVICE_H
#define MDNSSERVICE_H

#include <ArduinoLog.h> // https://github.com/thijse/Arduino-Log
#include <ESP8266mDNS.h> // https://github.com/esp8266/Arduino/blob/master/libraries/ESP8266mDNS/ESP8266mDNS.h
#include <ArduinoJson.h> // https://github.com/bblanchon/ArduinoJson

#include "Service.h"

namespace esp8266util {

	class MDNSService : public Service {

		public:

		bool isSetup();

	    bool available();

	    bool start();

		MDNSResponder& getMDNSResponder();

		bool setup(const char* domainName);

		const char* getDomainName();

		JsonObject& getDetails();

	  private:

		const char* _domainName;

		bool _setupDone = false;
		bool _available = false;
	};
}

#endif // MDNSSERVICE_H
