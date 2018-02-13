#ifndef FILE_SYSTEM_H
#define FILE_SYSTEM_H

#include <FS.h>          // https://github.com/esp8266/Arduino/tree/master/cores/esp8266/FS.h
#include <Log4Esp.h>     // https://github.com/hunsalz/log4Esp

#include <ArduinoJson.h> // https://github.com/bblanchon/ArduinoJson

namespace esp8266util {

class FileSystem {

public:

  bool begin();

  void end();

  JsonObject &getStorageDetails();

  JsonArray &getFileListing();

  static String formatBytes(size_t bytes);
};
} // namespace esp8266util

#if !defined(NO_GLOBAL_INSTANCES)
extern esp8266util::FileSystem FILESYSTEM;
#endif // NO_GLOBAL_INSTANCES

#endif // FILE_SYSTEM_H