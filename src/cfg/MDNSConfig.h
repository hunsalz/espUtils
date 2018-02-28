#ifndef MDNS_CONFIG_H
#define MDNS_CONFIG_H

#include <ESP8266mDNS.h> // https://github.com/esp8266/Arduino/blob/master/libraries/ESP8266mDNS/ESP8266mDNS.h

#include "Configurable.h"

namespace esp8266util {

class MDNSConfig : public Configurable {

public:

  bool begin(const char *domainName);

  void end();

  const char* getDomainName();

  const char* getConfigAsJson();

  const char* getDetails();

private:
  const char *_domainName;
};
} // namespace esp8266util

#if !defined(NO_GLOBAL_INSTANCES)
extern esp8266util::MDNSConfig MDNS_CFG;
#endif // NO_GLOBAL_INSTANCES

#endif // MDNS_CONFIG_H