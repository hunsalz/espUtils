#ifndef CONFIGURABLE_H
#define CONFIGURABLE_H

#include <Log4Esp.h>     // https://github.com/hunsalz/log4Esp
#include <FS.h>          // https://github.com/esp8266/Arduino/tree/master/cores/esp8266/FS.h

#include "polyfills/Json2String.h"

#include <ArduinoJson.h> // https://github.com/bblanchon/ArduinoJson

namespace esp8266utils {

class Configurable {

public:
  virtual const char* getConfigAsJson() = 0;

  JsonVariant read(const char *path);

  void write(const char *path);
};
} // namespace esp8266utils

#endif // CONFIGURABLE_H