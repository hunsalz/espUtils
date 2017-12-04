#include "Configurable.h"

namespace esp8266util {

  JsonVariant Configurable::read(const char* path) {

    JsonVariant json;
    if (FILESYSTEM.getFileSystem().exists(path)) {
      File file = FILESYSTEM.getFileSystem().open(path, "r+");
      DynamicJsonBuffer buffer;
      json = buffer.parse(file);
      if (json.success()) {
        Log.verbose(F("Loaded json config from file [%s]" CR), path);
      } else {
        Log.error(F("Malformed json config file [%s]" CR), path);
      }
      file.close();
    }

    return json;
  }

  void Configurable::write(const char* path) {

    JsonObject& json = getConfigAsJson();
    int length = json.measureLength() + 1;
    char payload[length];
    json.printTo(payload, length);

    File file = FILESYSTEM.getFileSystem().open(path, "w+");
    file.print(payload);
    file.flush();
    file.close();

    Log.verbose(F("Saved config to file [%s]" CR), path);
  }
}
