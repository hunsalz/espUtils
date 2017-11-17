#ifndef CONFIGURABLE_H
#define CONFIGURABLE_H

#include <ArduinoLog.h> // https://github.com/thijse/Arduino-Log
#include <ArduinoJson.h> // https://github.com/bblanchon/ArduinoJson

#include "FSService.h"

namespace esp8266util {

  class Configurable {

    public:

      virtual JsonObject& getConfigAsJson() = 0;

      JsonVariant read(const char* path);

      void write(const char* path);
  };
}

#endif // CONFIGURABLE_H