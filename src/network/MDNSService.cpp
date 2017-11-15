#include "MDNSService.h"

namespace esp8266util {

  bool MDNSService::available() {
    return _available;
  }

  bool MDNSService::begin(const char* domainName) {

    _domainName = domainName;
    
    if (!available()) {
      if (MDNS.begin(getDomainName())) {
        Log.verbose(F("MDNS enabled to http://%s.local" CR), getDomainName());
        _available = true;
      } else {
        Log.error(F("MDNS failed for http://%s.local" CR), getDomainName());
      }
    }

    return available();
  }

  MDNSResponder& MDNSService::getMDNSResponder() {
    return MDNS;
  }

  const char* MDNSService::getDomainName() {
    return _domainName;
  }

  JsonObject& MDNSService::getDetails() {

    DynamicJsonBuffer jsonBuffer;
    JsonObject &json = jsonBuffer.createObject();
    json[F("domainName")] = getDomainName();

    return json;
  }

  extern MDNSService MDNS_SERVICE = MDNSService();
}
