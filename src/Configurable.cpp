#include "Configurable.h"

namespace esp8266util {

JsonVariant Configurable::read(const char *path) {
  
  JsonVariant json;
  if (SPIFFS.exists(path)) {
    File file = SPIFFS.open(path, "r+");
    DynamicJsonBuffer buffer;
    json = buffer.parse(file);
    if (json.success()) {
      LOG.verbose(F("Loaded json config from file [%s]"), path);
    } else {
      LOG.error(F("Malformed json config file [%s]"), path);
    }
    file.close();
  }

  return json;
}

void Configurable::write(const char *path) {
  
  JsonObject &json = getConfigAsJson();
  int length = json.measureLength() + 1;
  char payload[length];
  json.printTo(payload, length);

  File file = SPIFFS.open(path, "w+");
  file.print(payload);
  file.flush();
  file.close();

  LOG.verbose(F("Saved config to file [%s]"), path);
}
} // namespace esp8266util
