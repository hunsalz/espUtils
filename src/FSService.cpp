#include "FSService.h"

namespace esp8266util {

  FSService::FSService() {}

  FSService::~FSService() {
    end();
  }

  bool FSService::isRunning() {
    return running;
  }

  bool FSService::begin() {

    if (!isRunning()) {
      if (SPIFFS.begin()) {
        Log.verbose(F("File system mounted." CR));
        running = true;
      } else {
        Log.warning(F("Mounting file system failed." CR));
      }
    }

    return isRunning();
  }

  bool FSService::end() {

    if (isRunning()) {
      SPIFFS.end();
      running = false;
    }

    return isRunning();
  }

  FS& FSService::getFS() {
    return SPIFFS;
  }

  JsonObject& FSService::getStorageDetails() {

    FSInfo fs_info;
    SPIFFS.info(fs_info);

    DynamicJsonBuffer jsonBuffer;
    JsonObject &json = jsonBuffer.createObject();
    json[F("totalBytes")] = fs_info.totalBytes;
    json[F("usedBytes")] = fs_info.usedBytes;
    json[F("blockSize")] = fs_info.blockSize;
    json[F("pageSize")] = fs_info.pageSize;
    json[F("maxOpenFiles")] = fs_info.maxOpenFiles;
    json[F("maxPathLength")] = fs_info.maxPathLength;

    return json;
  }

  JsonArray& FSService::getFileListing() {

    DynamicJsonBuffer jsonBuffer;
    JsonArray& json = jsonBuffer.createArray();
    // enumerate files
    Dir dir = SPIFFS.openDir("/");
    while (dir.next()) {
      String name = dir.fileName();
      String size = formatBytes(dir.fileSize());
      JsonObject& entry = json.createNestedObject();
      entry[F("name")] = name;
      entry[F("size")] = size;
      Log.verbose(F("Found file: name=%s, size=%s" CR), name.c_str(), size.c_str());
    }

    return json;
  }

  String FSService::formatBytes(size_t bytes) {

    if (bytes < 1024){
      return String(bytes) + "B";
    } else if (bytes < (1024 * 1024)){
      return String(bytes/1024.0) + "KB";
    } else if (bytes < (1024 * 1024 * 1024)){
      return String(bytes/1024.0/1024.0) + "MB";
    } else {
      return String(bytes/1024.0/1024.0/1024.0) + "GB";
    }
  }

  FSService FILESYSTEM = FSService();
}