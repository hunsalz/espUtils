#pragma once

namespace espUtils {

#define LOG_LEVEL 3

#ifdef ESP32
  #define DEBUG_ESP_PORT Serial
  // https://github.com/espressif/arduino-esp32/issues/1371
  #undef FPSTR
  #define FPSTR(pstr_pointer) (reinterpret_cast<const __FlashStringHelper *>(pstr_pointer))
#endif

#ifdef DEBUG_ESP_PORT
  #define LOGGER(baud) Logging::init(baud)
#else
  #define LOGGER(baud)
#endif

#ifdef DEBUG_ESP_PORT && LOG_LEVEL >= 1
  #define INFO(output) Logging::log(FPSTR(INFO), output)
  #define INFO_F(format, ...) Logging::log_F(FPSTR(INFO), format, ##__VA_ARGS__)
  #define INFO_P(output) Logging::log(FPSTR(INFO), output)
  #define INFO_FP(format, ...) Logging::log_FP(FPSTR(INFO), format, ##__VA_ARGS__)
#else
  #define INFO(...)
  #define INFO_F(...)
  #define INFO_P(...)
  #define INFO_FP(...)
#endif

#ifdef DEBUG_ESP_PORT && LOG_LEVEL >= 2
  #define VERBOSE(output) Logging::log(FPSTR(VERBOSE), output)
  #define VERBOSE_F(format, ...) Logging::log_F(FPSTR(VERBOSE), format, ##__VA_ARGS__)
  #define VERBOSE_P(output) Logging::log(FPSTR(VERBOSE), output)
  #define VERBOSE_FP(format, ...) Logging::log_FP(FPSTR(VERBOSE), format, ##__VA_ARGS__)
#else
  #define VERBOSE(...)
  #define VERBOSE_F(...)
  #define VERBOSE_P(...)
  #define VERBOSE_FP(...)
#endif

#ifdef DEBUG_ESP_PORT && LOG_LEVEL >= 3
  #define TRACE(output) Logging::log(FPSTR(TRACE), output)
  #define TRACE_F(format, ...) Logging::log_F(FPSTR(TRACE), format, ##__VA_ARGS__)
  #define TRACE_P(output) Logging::log(FPSTR(TRACE), output)
  #define TRACE_FP(format, ...) Logging::log_FP(FPSTR(TRACE), format, ##__VA_ARGS__)
#else
  #define TRACE(...)
  #define TRACE_F(...)
  #define TRACE_P(...)
  #define TRACE_FP(...)
#endif

#ifdef DEBUG_ESP_PORT
  #define WARNING(output) Logging::log(FPSTR(WARNING), output)
  #define WARNING_F(format, ...) Logging::log_F(FPSTR(WARNING), format, ##__VA_ARGS__)
  #define WARNING_P(output) Logging::log(FPSTR(WARNING), output)
  #define WARNING_FP(format, ...) Logging::log_FP(FPSTR(WARNING), format, ##__VA_ARGS__)
  #define ERROR(output) Logging::log_F(FPSTR(ERROR), output)
  #define ERROR_F(format, ...) Logging::log(FPSTR(ERROR), format, ##__VA_ARGS__)
  #define ERROR_P(output) Logging::log(FPSTR(ERROR), output)
  #define ERROR_FP(format, ...) Logging::log_FP(FPSTR(ERROR), format, ##__VA_ARGS__)
#else
  #define WARNING(...)
  #define WARNING_F(...)
  #define WARNING_P(...)
  #define WARNING_FP(...)
  #define ERROR(...)
  #define ERROR_F(...)
  #define ERROR_P(...)
  #define ERROR_FP(...)
#endif

static const char INFO[] PROGMEM = "INFO";
static const char VERBOSE[] PROGMEM = "VERBOSE";
static const char TRACE[] PROGMEM = "TRACE";
static const char WARNING[] PROGMEM = "WARNING";
static const char ERROR[] PROGMEM = "ERROR";

static char LOG_SEPARATOR = '|';

class Logging {

  public:

    static void init(unsigned long baud, bool debug = false) {
  
      DEBUG_ESP_PORT.begin(baud);
      DEBUG_ESP_PORT.setDebugOutput(debug);
      while (!DEBUG_ESP_PORT && !DEBUG_ESP_PORT.available()) {
      };
      DEBUG_ESP_PORT.println();
    }

    static void log_F(const __FlashStringHelper* prefix, const char* format, ...) {
  
      va_list args;
      va_start(args, format);
      // determine buffer length for formatted data
      size_t length = vsnprintf(NULL, 0, format, args) + 1;
      char buffer[length];
      // write formatted data to buffer
      vsnprintf(buffer, length, format, args);
      va_end(args);

      log(prefix, buffer);
    }

    static void log_FP(const __FlashStringHelper* prefix, const __FlashStringHelper* format, ...) {
  
      // copy PGM_P into a char*
      char* formatBuffer = new char[strlen_P((PGM_P)format) + 1];
      strcpy_P(formatBuffer, (PGM_P)format);

      va_list args;
      va_start(args, format);
      // determine buffer length for formatted data
      size_t length = vsnprintf(NULL, 0, formatBuffer, args) + 1;
      char buffer[length];
      // write formatted data to buffer
      vsnprintf(buffer, length, formatBuffer, args);
      va_end(args);

      log(prefix, buffer);
    }

    template<typename T> static void log(const __FlashStringHelper* prefix, T output) {

      DEBUG_ESP_PORT.print(prefix);
      DEBUG_ESP_PORT.print(LOG_SEPARATOR);
      DEBUG_ESP_PORT.print(millis());
      DEBUG_ESP_PORT.print(LOG_SEPARATOR);
      DEBUG_ESP_PORT.print(ESP.getFreeHeap());
      DEBUG_ESP_PORT.print(LOG_SEPARATOR);
      DEBUG_ESP_PORT.println(output);
    }
};
} // namespace espUtils