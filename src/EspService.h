#ifndef ESPSERVICE_H
#define ESPSERVICE_H

#ifdef ESP8266
extern "C" {
#include "user_interface.h" // Expressif ESP8266 Non-OS API
}
#endif

#include <ESP.h>         // https://github.com/esp8266/Arduino/blob/master/cores/esp8266/Esp.h
#include <Log4Esp.h>     // https://github.com/hunsalz/log4Esp
// include ArduinoJson.h last - order of includes matters
#include <ArduinoJson.h> // https://github.com/bblanchon/ArduinoJson

#include "Service.h"

using log4Esp::LOG;

namespace esp8266util {

class EspService : public Service {

public:
  bool available();

  EspClass &getESP();

  long unsigned getRemainingLoopInterval();

  bool nextLoopInterval();

  int getLoopInterval();

  void setLoopInterval(uint16_t milliseconds);

  void deepSleep();

  int getDeepSleepInterval();
  void setDeepSleepInterval(uint16_t milliseconds);

  JsonObject &getDetails();

private:
  uint16_t _loopInterval = 5000; // milliseconds
  const int MIN_LOOP_INTERVAL = 500; // milliseconds
  unsigned long _lastLoopInterval = millis(); // milliseconds
  uint16_t _deepSleepInterval = 60000; // milliseconds
  const int MIN_SLEEP_INTERVAL = 30000; // milliseconds
};

extern EspService SYSTEM;
} // namespace esp8266util

#endif // ESPSERVICE_H