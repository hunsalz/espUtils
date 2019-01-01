#ifndef SYS_CONFIG_H
#define SYS_CONFIG_H

#ifdef ESP8266
extern "C" {
#include "user_interface.h" // Expressif ESP8266 Non-OS API
}
#endif

#include <ArduinoJson.h>    // https://github.com/bblanchon/ArduinoJson
#include <ESP.h>            // https://github.com/esp8266/Arduino/blob/master/cores/esp8266/Esp.h

#include "../Logging.hpp"
#include "polyfills/Json2String.h"

namespace esp8266utils {

class SysConfig {

public:
  long unsigned getRemainingLoopInterval();

  bool nextLoopInterval();

  int getLoopInterval();

  void setLoopInterval(uint16_t milliseconds);

  String getDetails();

private:
  uint16_t _loopInterval = 5000;              // milli seconds
  const int MIN_LOOP_INTERVAL = 500;          // milli seconds
  unsigned long _lastLoopInterval = millis(); // milli seconds
  uint16_t _deepSleepInterval = 60000;        // milli seconds
  const int MIN_SLEEP_INTERVAL = 30000;       // milli seconds
};
} // namespace esp8266utils

#if !defined(NO_GLOBAL_INSTANCES)
extern esp8266utils::SysConfig SYS_CFG;
#endif // NO_GLOBAL_INSTANCES

#endif // SYS_CONFIG_H