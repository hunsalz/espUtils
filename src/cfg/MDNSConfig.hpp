#pragma once

#include <ArduinoJson.h>  // https://github.com/bblanchon/ArduinoJson
#include <ESP8266mDNS.h>  // https://github.com/esp8266/Arduino/blob/master/libraries/ESP8266mDNS/ESP8266mDNS.h

#include "../Logging.hpp"

namespace esp8266utils {

class MDNSConfig {
 
 public:
  
  bool begin(const char* hostName) {
    
    _hostName = hostName;
    bool available = MDNS.begin(getHostName());
    if (available) {
      VERBOSE_FP(F("MDNS enabled to http://%s.local"), getHostName());
    } else {
      ERROR_FP(F("MDNS failed for http://%s.local"), getHostName());
    }
  }

  const char* getHostName() {
    return _hostName;
  }

  size_t serialize(String& output) {

    DynamicJsonDocument doc;
    JsonObject object = doc.to<JsonObject>();
    object[F("hostName")] = getHostName();
    serializeJson(object, output);
    return measureJson(object);
  }


 private:

  const char* _hostName;
};
}  // namespace esp8266utils

#if !defined(NO_GLOBAL_INSTANCES)
extern esp8266utils::MDNSConfig MDNS_CFG;
#endif  // NO_GLOBAL_INSTANCES