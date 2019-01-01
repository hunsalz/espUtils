#include "SysConfig.h"

namespace esp8266utils {

long unsigned SysConfig::getRemainingLoopInterval() {

  long unsigned nextLoopInterval = _lastLoopInterval + getLoopInterval();
  long unsigned now = millis();

  if (nextLoopInterval > now) {
    return nextLoopInterval - now;
  } else {
    _lastLoopInterval = millis();
    return 0;
  }
}

bool SysConfig::nextLoopInterval() { 
  return !getRemainingLoopInterval(); 
}

int SysConfig::getLoopInterval() { 
  return _loopInterval; 
}

void SysConfig::setLoopInterval(uint16_t milliseconds) {

  if (milliseconds < MIN_LOOP_INTERVAL) {
    milliseconds = MIN_LOOP_INTERVAL;
    WARNING_MSG_P(F("Loop interval limited to %d milliseconds."), milliseconds);
  } else {
    VERBOSE_MSG_P(F("Loop interval set to %d milliseconds."), milliseconds);
  }
  _loopInterval = milliseconds;
}

String SysConfig::getDetails() {

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
} // namespace esp8266utils

#if !defined(NO_GLOBAL_INSTANCES)
esp8266utils::SysConfig SYS_CFG;
#endif // NO_GLOBAL_INSTANCES