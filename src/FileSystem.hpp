#pragma once

#include <ArduinoJson.h>  // https://github.com/bblanchon/ArduinoJson
#include <FS.h>           // https://github.com/espressif/arduino-esp32/blob/master/libraries/FS/src/FS.h
                          // https://github.com/esp8266/Arduino/tree/master/cores/esp8266/FS.h

#include "Logging.hpp"

namespace espUtils {

class FileSystem {
 
 public:
  
  bool begin() {
    
    #ifdef ESP32

    // TODO ESP32
    bool available = false;

    #else
    bool available = SPIFFS.begin();
    if (available) {
      VERBOSE_FP(F("File system mounted."));
    } else {
      WARNING_FP(F("Mounting file system failed."));
    }
    #endif

    return available;
  }

  void end() {
    
    #ifdef ESP32

    // TODO ESP32

    #else
    SPIFFS.end();
    VERBOSE_FP(F("File system unmounted."));
    #endif
  }

  size_t serializeInfo(String& output) {

    DynamicJsonDocument doc;
    JsonObject object = doc.to<JsonObject>();
    #ifdef ESP32

    // TODO ESP32

    #else
    FSInfo fs_info;
    SPIFFS.info(fs_info);
    object[F("totalBytes")] = fs_info.totalBytes;
    object[F("usedBytes")] = fs_info.usedBytes;
    object[F("blockSize")] = fs_info.blockSize;
    object[F("pageSize")] = fs_info.pageSize;
    object[F("maxOpenFiles")] = fs_info.maxOpenFiles;
    object[F("maxPathLength")] = fs_info.maxPathLength;
    #endif
    
    serializeJson(object, output);
    return measureJson(object);
  }

  size_t serializeListing(String& output) {
    
    DynamicJsonDocument doc;
    JsonArray array = doc.to<JsonArray>();
    #ifdef ESP32

    // TODO ESP32

    #else
    // enumerate files
    Dir dir = SPIFFS.openDir("/");
    while (dir.next()) {
      String name = dir.fileName();
      String size = formatBytes(dir.fileSize());
      JsonObject entry = array.createNestedObject();
      entry[F("name")] = name;
      entry[F("size")] = size;
      VERBOSE_FP(F("Found file: name=%s, size=%s"), name.c_str(), size.c_str());
    }
    #endif
    
    serializeJson(array, output);
    return measureJson(array);
  }

  String formatBytes(size_t bytes) {
    
    if (bytes < 1024) {
      return String(bytes) + "B";
    } else if (bytes < (1024 * 1024)) {
      return String(bytes / 1024.0) + "KB";
    } else if (bytes < (1024 * 1024 * 1024)) {
      return String(bytes / 1024.0 / 1024.0) + "MB";
    } else {
      return String(bytes / 1024.0 / 1024.0 / 1024.0) + "GB";
    }
  }
};
}  // namespace espUtils