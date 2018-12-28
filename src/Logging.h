#ifndef LOGGING_H
#define LOGGING_H

#include <Arduino.h>

namespace esp8266utils {

#define LOG_SEPARATOR '|'

static const char INFO[] PROGMEM = "INFO";
static const char VERBOSE[] PROGMEM = "VERBOSE";
static const char ERROR[] PROGMEM = "ERROR";

class Logging {

  public:
    static void init(unsigned long baud);
    static void log(const __FlashStringHelper *prefix, const char *format, ...);
    static void log_P(const __FlashStringHelper *prefix, const __FlashStringHelper *format, ...);
};

inline void Logging::init(unsigned long baud) {
  
  DEBUG_ESP_PORT.begin(baud);
  DEBUG_ESP_PORT.setDebugOutput(false);
  while (!DEBUG_ESP_PORT && !DEBUG_ESP_PORT.available()) {
  };
  DEBUG_ESP_PORT.println();
}

inline void Logging::log(const __FlashStringHelper *prefix, const char *format, ...) {
  
  va_list args;
  va_start(args, format);
  // determine buffer length for formatted data
  size_t length = vsnprintf(NULL, 0, format, args) + 1;
  char buffer[length];
  // write formatted data to buffer
  vsnprintf(buffer, length, format, args);
  va_end(args);

  DEBUG_ESP_PORT.print(prefix);
  DEBUG_ESP_PORT.print(LOG_SEPARATOR);
  DEBUG_ESP_PORT.print(millis());
  DEBUG_ESP_PORT.print(LOG_SEPARATOR);
  DEBUG_ESP_PORT.print(ESP.getFreeHeap());
  DEBUG_ESP_PORT.print(LOG_SEPARATOR);
  DEBUG_ESP_PORT.println(buffer);
}

inline void Logging::log_P(const __FlashStringHelper *prefix, const __FlashStringHelper *format, ...) {
  
  // copy PGM_P into a char*
  char *buffer = new char[strlen_P((PGM_P)format) + 1];
  strcpy_P(buffer, (PGM_P)format);

  // proceed by using the standard function
  va_list args;
  va_start(args, format);
  Logging::log(prefix, buffer, args);
  va_end(args);
}

} // namespace esp8266utils

#endif  // LOGGING_H