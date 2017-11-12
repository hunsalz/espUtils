#include "MDNSService.h"

namespace esp8266util {

  MDNSService::~MDNSService() {
    stop();
  }

  bool MDNSService::isSetup() {
    return _setupDone;
  }

  bool MDNSService::isRunning() {
    return _running;
  }

  bool MDNSService::start() {

    if (isSetup()) {
      if (!isRunning()) {
        if (MDNS.begin(getDomainName())) {
          Log.verbose(F("MDNS enabled to http://%s.local" CR), getDomainName());
          _running = true;
        } else {
          Log.error(F("MDNS failed for http://%s.local" CR), getDomainName());
        }
      }
    } else {
      Log.error("Call setup() first.");
    }

    return isRunning();
  }

  bool MDNSService::stop() {

    // TODO how to disable MDNS ?

    return isRunning();
  }

  MDNSResponder* MDNSService::getMDNSResponder() {
    return &MDNS;
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
