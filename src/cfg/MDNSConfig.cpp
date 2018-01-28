#include "MDNSConfig.h"

namespace esp8266util {

bool MDNSConfig::begin(const char *domainName) {
  
  _domainName = domainName;
  bool available = MDNS.begin(getDomainName());
  if (available) {
    LOG.verbose(F("MDNS enabled to http://%s.local"), getDomainName());
  } else {
    LOG.error(F("MDNS failed for http://%s.local"), getDomainName());
  }
}

const char *MDNSConfig::getDomainName() { return _domainName; }

JsonObject &MDNSConfig::getDetails() {
  
  DynamicJsonBuffer jsonBuffer;
  JsonObject &json = jsonBuffer.createObject();
  json[F("domainName")] = getDomainName();

  return json;
}
} // namespace esp8266util

#if !defined(NO_GLOBAL_INSTANCES)
esp8266util::MDNSConfig MDNS_CFG;
#endif // NO_GLOBAL_INSTANCES