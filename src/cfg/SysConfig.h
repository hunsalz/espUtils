#ifndef SYS_CONFIG_H
#define SYS_CONFIG_H

#ifdef ESP8266
extern "C" {
#include "user_interface.h" // Expressif ESP8266 Non-OS API
}
#endif

#include <ESP.h>     // https://github.com/esp8266/Arduino/blob/master/cores/esp8266/Esp.h

#include "Configurable.h"

namespace esp8266utils {

class SysConfig : public Configurable {

public:
  long unsigned getRemainingLoopInterval();

  bool nextLoopInterval();

  int getLoopInterval();

  void setLoopInterval(uint16_t milliseconds);

  void deepSleep();

  int getDeepSleepInterval();
  void setDeepSleepInterval(uint16_t milliseconds);

  String getConfigAsJson();

  String getDetails();

private:
  uint16_t _loopInterval = 5000;              // milliseconds
  const int MIN_LOOP_INTERVAL = 500;          // milliseconds
  unsigned long _lastLoopInterval = millis(); // milliseconds
  uint16_t _deepSleepInterval = 60000;        // milliseconds
  const int MIN_SLEEP_INTERVAL = 30000;       // milliseconds
};
} // namespace esp8266utils

#if !defined(NO_GLOBAL_INSTANCES)
extern esp8266utils::SysConfig SYS_CFG;
#endif // NO_GLOBAL_INSTANCES

#endif // SYS_CONFIG_H