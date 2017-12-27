#ifndef CONFIGURABLE_H
#define CONFIGURABLE_H

#include <ArduinoJson.h> // https://github.com/bblanchon/ArduinoJson
#include <Logger.h> // https://github.com/hunsalz/log4arduino

#include "FSService.h"

using log4arduino::LOG;

namespace esp8266util {

  class Configurable {

    public:

      virtual JsonObject& getConfigAsJson() = 0;

      JsonVariant read(const char* path);

      void write(const char* path);
  };
}

#endif // CONFIGURABLE_H