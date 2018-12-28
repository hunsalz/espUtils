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

bool SysConfig::nextLoopInterval() { return !getRemainingLoopInterval(); }

int SysConfig::getLoopInterval() { return _loopInterval; }

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

  DynamicJsonBuffer jsonBuffer;
  JsonObject &json = jsonBuffer.createObject();
  json[F("vcc")] = ESP.getVcc();
  json[F("heap")] = ESP.getFreeHeap();
  json[F("chipId")] = ESP.getChipId();
  json[F("sdkVersion")] = ESP.getSdkVersion();
  json[F("coreVersion")] = ESP.getCoreVersion();
  json[F("bootVersion")] = ESP.getBootVersion();
  json[F("bootMode")] = ESP.getBootMode();
  json[F("cpuFreqMHz")] = ESP.getCpuFreqMHz();
  json[F("flashChipId")] = ESP.getFlashChipId();
  json[F("flashChipRealSize")] = ESP.getFlashChipRealSize();
  json[F("flashChipSize")] = ESP.getFlashChipSize();
  json[F("flashChipSpeed")] = ESP.getFlashChipSpeed();
  json[F("flashChipMode")] = ESP.getFlashChipMode();
  json[F("flashChipSizeByChipId")] = ESP.getFlashChipSizeByChipId();
  json[F("sketchSize")] = ESP.getSketchSize();
  json[F("sketchMD5")] = ESP.getSketchMD5();
  json[F("freeSketchSpace")] = ESP.getFreeSketchSpace();
  json[F("resetReason")] = ESP.getResetReason();
  json[F("resetInfo")] = ESP.getResetInfo();
  json[F("cycleCount")] = ESP.getCycleCount();
  json[F("uptime")] = millis();
  json[F("loopInterval")] = getLoopInterval();
  json[F("remainingLoopInterval")] = getRemainingLoopInterval();

  return esp8266utils::toString(json);
}
} // namespace esp8266utils

#if !defined(NO_GLOBAL_INSTANCES)
esp8266utils::SysConfig SYS_CFG;
#endif // NO_GLOBAL_INSTANCES