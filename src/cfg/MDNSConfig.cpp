#include "MDNSConfig.h"

namespace esp8266utils {

bool MDNSConfig::begin(const char *domainName) {
  
  _domainName = domainName;
  bool available = MDNS.begin(getDomainName());
  if (available) {
    VERBOSE_MSG_P(F("MDNS enabled to http://%s.local"), getDomainName());
  } else {
    ERROR_MSG_P(F("MDNS failed for http://%s.local"), getDomainName());
  }
}

const char* MDNSConfig::getDomainName() { 
  return _domainName; 
}

String MDNSConfig::getDetails() {
  
  DynamicJsonDocument doc;
  JsonObject object = doc.to<JsonObject>();
  object[F("domainName")] = getDomainName();

  return esp8266utils::toString(object);
}
} // namespace esp8266utils

#if !defined(NO_GLOBAL_INSTANCES)
esp8266utils::MDNSConfig MDNS_CFG;
#endif // NO_GLOBAL_INSTANCES