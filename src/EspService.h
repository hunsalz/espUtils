#ifndef ESPSERVICE_H
#define ESPSERVICE_H

#ifdef ESP8266
extern "C" {
  #include "user_interface.h" // Expressif ESP8266 Non-OS API
}
#endif

#include <ArduinoLog.h> // https://github.com/thijse/Arduino-Log
#include <ArduinoJson.h> // https://github.com/bblanchon/ArduinoJson
#include <ESP.h> // https://github.com/esp8266/Arduino/blob/master/cores/esp8266/Esp.h

#include "Service.h"

namespace esp8266util {

  class EspService : public Service {

    public:

      bool isRunning();

      EspClass& getESP();

      JsonObject& getDetails();
  };

  extern EspService SYSTEM;
}

#endif // ESPSERVICE_H