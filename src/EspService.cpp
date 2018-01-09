#include "EspService.h"

namespace esp8266util {

  bool EspService::available() {
    return true;
  }

  EspClass& EspService::getESP() {
    return ESP;
  }

  long unsigned EspService::getRemainingLoopInterval() {
    
    long unsigned nextLoopInterval = _lastLoopInterval + getLoopInterval();
    long unsigned now = millis();

    if (nextLoopInterval > now) {
      return nextLoopInterval - now;
    } else {
      _lastLoopInterval = millis();
      return 0;
    }
  }

  bool EspService::nextLoopInterval() {
    return !getRemainingLoopInterval();
  }

  int EspService::getLoopInterval() {
    return _loopInterval;
  }

  void EspService::setLoopInterval(int milliseconds) {

    if (milliseconds < 500) {
      milliseconds = 500;
      LOG.warning(F("Loop interval limited to %d milliseconds."), milliseconds);
    } else {
      LOG.verbose(F("Loop interval set to %d milliseconds."), milliseconds);
    }
    _loopInterval = milliseconds;
  }

  void EspService::deepSleep() {
    ESP.deepSleep(getDeepSleepInterval());
  }

  int EspService::getDeepSleepInterval() {
    return _deepSleepInterval;
  }

  void EspService::setDeepSleepInterval(int milliseconds) {
    
    if (milliseconds < 30000) {
      milliseconds = 30000;
      LOG.warning(F("Deep sleep interval limited to %d milliseconds."), milliseconds);
    } else {
      LOG.verbose(F("Deep sleep interval set to %d milliseconds."), milliseconds);
    }
    _deepSleepInterval = milliseconds;
  }

  JsonObject& EspService::getDetails() {

    DynamicJsonBuffer jsonBuffer;
    JsonObject &json = jsonBuffer.createObject();
    json[F("vcc")] = ESP.getVcc();
    json[F("heap")] = ESP.getFreeHeap();
    json[F("chipId")] = ESP.getChipId();
    json[F("sdkVersion")] = ESP.getSdkVersion();
    json[F("coreVersion")] =  ESP.getCoreVersion();
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
    json[F("deepSleepInterval")] = getDeepSleepInterval();

    return json;
  }

  EspService SYSTEM = EspService();
}
