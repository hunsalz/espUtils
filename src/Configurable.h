#ifndef CONFIGURABLE_H
#define CONFIGURABLE_H

#include <Log4Esp.h>     // https://github.com/hunsalz/log4Esp
#include <FS.h>          // https://github.com/esp8266/Arduino/tree/master/cores/esp8266/FS.h

#include <ArduinoJson.h> // https://github.com/bblanchon/ArduinoJson

namespace esp8266util {

class Configurable {

public:
  virtual JsonObject &getConfigAsJson() = 0;

  JsonVariant read(const char *path);

  void write(const char *path);
};
} // namespace esp8266util

#endif // CONFIGURABLE_H