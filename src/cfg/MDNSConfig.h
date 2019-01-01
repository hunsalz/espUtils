#ifndef MDNS_CONFIG_H
#define MDNS_CONFIG_H

#include <ArduinoJson.h> // https://github.com/bblanchon/ArduinoJson
#include <ESP8266mDNS.h> // https://github.com/esp8266/Arduino/blob/master/libraries/ESP8266mDNS/ESP8266mDNS.h

#include "../Logging.hpp"
#include "polyfills/Json2String.h"

namespace esp8266utils {

class MDNSConfig {

public:

  bool begin(const char *domainName);

  void end();

  const char* getDomainName();

  String getDetails();

private:
  const char *_domainName;
};
} // namespace esp8266utils

#if !defined(NO_GLOBAL_INSTANCES)
extern esp8266utils::MDNSConfig MDNS_CFG;
#endif // NO_GLOBAL_INSTANCES

#endif // MDNS_CONFIG_H