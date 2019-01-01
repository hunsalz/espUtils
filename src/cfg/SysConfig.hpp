#pragma once

#ifdef ESP8266
extern "C" {
#include "user_interface.h"  // Expressif ESP8266 Non-OS API
}
#endif

#include <ArduinoJson.h>  // https://github.com/bblanchon/ArduinoJson
#include <ESP.h>  // https://github.com/esp8266/Arduino/blob/master/cores/esp8266/Esp.h

#include "../Logging.hpp"
#include "polyfills/Json2String.h"

namespace esp8266utils {

class SysConfig {
 
 public:
  
  long unsigned getRemainingLoopInterval() {
    long unsigned nextLoopInterval = _lastLoopInterval + getLoopInterval();
    long unsigned now = millis();

    if (nextLoopInterval > now) {
      return nextLoopInterval - now;
    } else {
      _lastLoopInterval = millis();
      return 0;
    }
  }

  bool nextLoopInterval() {
    return !getRemainingLoopInterval();
  }

  int getLoopInterval() {
    return _loopInterval;
  }

  void setLoopInterval(uint16_t milliseconds) {
    if (milliseconds < MIN_LOOP_INTERVAL) {
      milliseconds = MIN_LOOP_INTERVAL;
      WARNING_MSG_P(F("Loop interval limited to %d milliseconds."),
                    milliseconds);
    } else {
      VERBOSE_MSG_P(F("Loop interval set to %d milliseconds."), milliseconds);
    }
    _loopInterval = milliseconds;
  }

  String getDetails() {
    
    DynamicJsonDocument doc;
    JsonObject object = doc.to<JsonObject>();
    object[F("vcc")] = ESP.getVcc();
    object[F("heap")] = ESP.getFreeHeap();
    object[F("chipId")] = ESP.getChipId();
    object[F("sdkVersion")] = ESP.getSdkVersion();
    object[F("coreVersion")] = ESP.getCoreVersion();
    object[F("bootVersion")] = ESP.getBootVersion();
    object[F("bootMode")] = ESP.getBootMode();
    object[F("cpuFreqMHz")] = ESP.getCpuFreqMHz();
    object[F("flashChipId")] = ESP.getFlashChipId();
    object[F("flashChipRealSize")] = ESP.getFlashChipRealSize();
    object[F("flashChipSize")] = ESP.getFlashChipSize();
    object[F("flashChipSpeed")] = ESP.getFlashChipSpeed();
    object[F("flashChipMode")] = ESP.getFlashChipMode();
    object[F("flashChipSizeByChipId")] = ESP.getFlashChipSizeByChipId();
    object[F("sketchSize")] = ESP.getSketchSize();
    object[F("sketchMD5")] = ESP.getSketchMD5();
    object[F("freeSketchSpace")] = ESP.getFreeSketchSpace();
    object[F("resetReason")] = ESP.getResetReason();
    object[F("resetInfo")] = ESP.getResetInfo();
    object[F("cycleCount")] = ESP.getCycleCount();
    object[F("uptime")] = millis();
    object[F("loopInterval")] = getLoopInterval();
    object[F("remainingLoopInterval")] = getRemainingLoopInterval();
    return esp8266utils::toString(object);
  }

 private:
  
  uint16_t _loopInterval = 5000;               // milli seconds
  const int MIN_LOOP_INTERVAL = 500;           // milli seconds
  unsigned long _lastLoopInterval = millis();  // milli seconds
  uint16_t _deepSleepInterval = 60000;         // milli seconds
  const int MIN_SLEEP_INTERVAL = 30000;        // milli seconds
};
}  // namespace esp8266utils

#if !defined(NO_GLOBAL_INSTANCES)
extern esp8266utils::SysConfig SYS_CFG;
#endif  // NO_GLOBAL_INSTANCES