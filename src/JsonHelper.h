#ifndef JSONHELPER_H
#define JSONHELPER_H

#include <ArduinoJson.h> // https://github.com/bblanchon/ArduinoJson

namespace esp8266util {

inline String toString(JsonObject &json) {
  
  int length = json.measureLength() + 1;
  char str[length];
  json.printTo(str, length);

  return String(str);
}

inline String toString(JsonArray &json) {

  int length = json.measureLength() + 1;
  char str[length];
  json.printTo(str, length);

  return String(str);
}
} // namespace esp8266util

#endif // JSONHELPER_H