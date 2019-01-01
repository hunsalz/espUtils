#include "FileSystem.h"

namespace esp8266utils {

bool FileSystem::begin() {

  bool available = SPIFFS.begin();
  if (available) {
    VERBOSE_MSG_P(F("File system mounted."));
  } else {
    WARNING_MSG_P(F("Mounting file system failed."));
  }

  return available;
}

void FileSystem::end() {

  SPIFFS.end();
  VERBOSE_MSG_P(F("File system unmounted."));
}

String FileSystem::getStorageDetails() {

  FSInfo fs_info;
  SPIFFS.info(fs_info);

  DynamicJsonDocument doc;
  JsonObject object = doc.to<JsonObject>();
  object[F("totalBytes")] = fs_info.totalBytes;
  object[F("usedBytes")] = fs_info.usedBytes;
  object[F("blockSize")] = fs_info.blockSize;
  object[F("pageSize")] = fs_info.pageSize;
  object[F("maxOpenFiles")] = fs_info.maxOpenFiles;
  object[F("maxPathLength")] = fs_info.maxPathLength;

  return esp8266utils::toString(object);
}

String FileSystem::getFileListing() {

  DynamicJsonDocument doc;
  JsonArray array = doc.to<JsonArray>();
  // enumerate files
  Dir dir = SPIFFS.openDir("/");
  while (dir.next()) {
    String name = dir.fileName();
    String size = formatBytes(dir.fileSize());
    JsonObject entry = array.createNestedObject();
    entry[F("name")] = name;
    entry[F("size")] = size;
    VERBOSE_MSG_P(F("Found file: name=%s, size=%s"), name.c_str(), size.c_str());
  }

  return esp8266utils::toString(array);
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
} // namespace esp8266utils

#if !defined(NO_GLOBAL_INSTANCES)
esp8266utils::FileSystem FILESYSTEM;
#endif // NO_GLOBAL_INSTANCES