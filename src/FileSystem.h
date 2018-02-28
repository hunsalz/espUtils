#ifndef FILE_SYSTEM_H
#define FILE_SYSTEM_H

#include <FS.h>          // https://github.com/esp8266/Arduino/tree/master/cores/esp8266/FS.h
#include <Log4Esp.h>     // https://github.com/hunsalz/log4Esp

#include "polyfills/Json2String.h"

#include <ArduinoJson.h> // https://github.com/bblanchon/ArduinoJson

namespace esp8266utils {

class FileSystem {

public:

  bool begin();

  void end();

  const char* getStorageDetails();

  const char* getFileListing();

  static String formatBytes(size_t bytes);
};
} // namespace esp8266utils

#if !defined(NO_GLOBAL_INSTANCES)
extern esp8266utils::FileSystem FILESYSTEM;
#endif // NO_GLOBAL_INSTANCES

#endif // FILE_SYSTEM_H