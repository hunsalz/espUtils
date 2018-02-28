#ifndef JSON_2_STRING_H
#define JSON_2_STRING_H

#include <ArduinoJson.h> // https://github.com/bblanchon/ArduinoJson

namespace esp8266utils {

inline char* toString(const JsonVariant &json) {
  
  int length = json.measureLength() + 1;
  char* buf = new char[length];
  json.printTo(buf, length);

  return buf;
}
} // namespace esp8266utils

#endif // JSON_2_STRING_H