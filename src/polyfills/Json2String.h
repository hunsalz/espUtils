#pragma once

#include <ArduinoJson.h> // https://github.com/bblanchon/ArduinoJson

namespace esp8266utils {

inline String toString(const JsonVariant &json) {

  String output;
  serializeJson(json, output);

  return output;
}

} // namespace esp8266utils