#pragma once

#include <Arduino.h>

#define LOG_SEPARATOR '|'
#define LOG_LEVEL 3

#if defined DEBUG_ESP_PORT && LOG_LEVEL >= 1
#define INFO_MSG(format, ...) esp8266utils::Logging::log(FPSTR(esp8266utils::INFO), format, ##__VA_ARGS__)
#define INFO_MSG_P(format, ...) esp8266utils::Logging::log_P(FPSTR(esp8266utils::INFO), format, ##__VA_ARGS__)
#else
#define INFO_MSG(...)
#define INFO_MSG_P(...)
#endif

#if defined DEBUG_ESP_PORT && LOG_LEVEL >= 2
#define VERBOSE_MSG(format, ...) esp8266utils::Logging::log(FPSTR(esp8266utils::VERBOSE), format, ##__VA_ARGS__)
#define VERBOSE_MSG_P(format, ...) esp8266utils::Logging::log_P(FPSTR(esp8266utils::VERBOSE), format, ##__VA_ARGS__)
#else
#define VERBOSE_MSG(...)
#define VERBOSE_MSG_P(...)
#endif

#if defined DEBUG_ESP_PORT && LOG_LEVEL >= 3
#define TRACE_MSG(format, ...) esp8266utils::Logging::log(FPSTR(esp8266utils::TRACE), format, ##__VA_ARGS__)
#define TRACE_MSG_P(format, ...) esp8266utils::Logging::log_P(FPSTR(esp8266utils::TRACE), format, ##__VA_ARGS__)
#else
#define TRACE_MSG(...)
#define TRACE_MSG_P(...)
#endif

#ifdef DEBUG_ESP_PORT
#define WARNING_MSG(format, ...) esp8266utils::Logging::log(FPSTR(esp8266utils::WARNING), format, ##__VA_ARGS__)
#define WARNING_MSG_P(format, ...) esp8266utils::Logging::log_P(FPSTR(esp8266utils::WARNING), format, ##__VA_ARGS__)
#define ERROR_MSG(format, ...) esp8266utils::Logging::log(FPSTR(esp8266utils::ERROR), format, ##__VA_ARGS__)
#define ERROR_MSG_P(format, ...) esp8266utils::Logging::log_P(FPSTR(esp8266utils::ERROR), format, ##__VA_ARGS__)
#else
#define WARNING_MSG(...)
#define WARNING_MSG_P(...)
#define ERROR_MSG(...)
#define ERROR_MSG_P(...)
#endif

namespace esp8266utils {

static const char INFO[] PROGMEM = "INFO";
static const char VERBOSE[] PROGMEM = "VERBOSE";
static const char TRACE[] PROGMEM = "TRACE";
static const char WARNING[] PROGMEM = "WARNING";
static const char ERROR[] PROGMEM = "ERROR";

class Logging {

  public:
    static void init(unsigned long baud);
    static void log(const __FlashStringHelper *prefix, const char *format, ...);
    static void log_P(const __FlashStringHelper *prefix, const __FlashStringHelper *format, ...);
};

inline void Logging::init(unsigned long baud) {
  
  #ifdef DEBUG_ESP_PORT
  DEBUG_ESP_PORT.begin(baud);
  DEBUG_ESP_PORT.setDebugOutput(false);
  while (!DEBUG_ESP_PORT && !DEBUG_ESP_PORT.available()) {
  };
  DEBUG_ESP_PORT.println();
  #endif
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

  #ifdef DEBUG_ESP_PORT
  DEBUG_ESP_PORT.print(prefix);
  DEBUG_ESP_PORT.print(LOG_SEPARATOR);
  DEBUG_ESP_PORT.print(millis());
  DEBUG_ESP_PORT.print(LOG_SEPARATOR);
  DEBUG_ESP_PORT.print(ESP.getFreeHeap());
  DEBUG_ESP_PORT.print(LOG_SEPARATOR);
  DEBUG_ESP_PORT.println(buffer);
  #endif
}

inline void Logging::log_P(const __FlashStringHelper *prefix, const __FlashStringHelper *format, ...) {
  
  // copy PGM_P into a char*
  char *formatBuffer = new char[strlen_P((PGM_P)format) + 1];
  strcpy_P(formatBuffer, (PGM_P)format);

  va_list args;
  va_start(args, format);
  // determine buffer length for formatted data
  size_t length = vsnprintf(NULL, 0, formatBuffer, args) + 1;
  char buffer[length];
  // write formatted data to buffer
  vsnprintf(buffer, length, formatBuffer, args);
  va_end(args);

  #ifdef DEBUG_ESP_PORT
  DEBUG_ESP_PORT.print(prefix);
  DEBUG_ESP_PORT.print(LOG_SEPARATOR);
  DEBUG_ESP_PORT.print(millis());
  DEBUG_ESP_PORT.print(LOG_SEPARATOR);
  DEBUG_ESP_PORT.print(ESP.getFreeHeap());
  DEBUG_ESP_PORT.print(LOG_SEPARATOR);
  DEBUG_ESP_PORT.println(buffer);
  #endif
}

} // namespace esp8266utils