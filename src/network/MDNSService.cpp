#include "MDNSService.h"

namespace esp8266util {

  bool MDNSService::isSetup() {
    return _setupDone;
  }

  bool MDNSService::available() {
    return _available;
  }

  bool MDNSService::start() {

    if (isSetup()) {
      if (!available()) {
        if (MDNS.begin(getDomainName())) {
          Log.verbose(F("MDNS enabled to http://%s.local" CR), getDomainName());
          _available = true;
        } else {
          Log.error(F("MDNS failed for http://%s.local" CR), getDomainName());
        }
      }
    } else {
      Log.error("Call setup() first.");
    }

    return available();
  }

  MDNSResponder& MDNSService::getMDNSResponder() {
    return MDNS;
  }

  bool MDNSService::setup(const char* domainName) {

    _domainName = domainName;
    _setupDone = true;

    return isSetup();
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
}
