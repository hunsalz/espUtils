#ifndef MDNSSERVICE_H
#define MDNSSERVICE_H

#include <ESP8266mDNS.h> // https://github.com/esp8266/Arduino/blob/master/libraries/ESP8266mDNS/ESP8266mDNS.h
#include <Log4Esp.h>	 // https://github.com/hunsalz/log4Esp
#include <ArduinoJson.h> // https://github.com/bblanchon/ArduinoJson

#include "Service.h"

using log4Esp::LOG;

namespace esp8266util
{

class MDNSService : public Service
{

  public:
	bool available();

	bool begin(const char *domainName);

	MDNSResponder &getMDNSResponder();

	const char *getDomainName();

	JsonObject &getDetails();

  private:
	const char *_domainName;

	bool _available = false;
};

extern MDNSService MDNS_SERVICE;
}

#endif // MDNSSERVICE_H
