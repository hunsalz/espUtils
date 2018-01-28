#include "FileSystem.h"

namespace esp8266util {

bool FileSystem::begin() {

  bool available = SPIFFS.begin();
  if (available) {
    LOG.verbose(F("File system mounted."));
  } else {
    LOG.warning(F("Mounting file system failed."));
  }

  return available;
}

void FileSystem::end() {

  SPIFFS.end();
  LOG.verbose(F("File system unmounted."));
}

JsonObject &FileSystem::getStorageDetails() {

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

JsonArray &FileSystem::getFileListing() {

  DynamicJsonBuffer jsonBuffer;
  JsonArray &json = jsonBuffer.createArray();
  // enumerate files
  Dir dir = SPIFFS.openDir("/");
  while (dir.next()) {
    String name = dir.fileName();
    String size = formatBytes(dir.fileSize());
    JsonObject &entry = json.createNestedObject();
    entry[F("name")] = name;
    entry[F("size")] = size;
    LOG.verbose(F("Found file: name=%s, size=%s"), name.c_str(), size.c_str());
  }

  return json;
}

String FileSystem::formatBytes(size_t bytes) {
  
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
} // namespace esp8266util

#if !defined(NO_GLOBAL_INSTANCES)
esp8266util::FileSystem FILESYSTEM;
#endif // NO_GLOBAL_INSTANCES