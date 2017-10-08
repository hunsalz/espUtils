#include "EspService.h"

namespace esp8266util {

  EspService::EspService() {
  }

  EspService::~EspService() {
    stop();
  }

  bool EspService::start() {

    if (!isRunning()) {
      // ...
    }

    return isRunning();
  }

  bool EspService::stop() {

    if (isRunning()) {
      running = false;
    }

    return isRunning();
  }

  JsonObject& EspService::getDetails() {

    DynamicJsonBuffer jsonBuffer;
    JsonObject &json = jsonBuffer.createObject();
    // ESP8266 data from https://github.com/esp8266/Arduino/blob/master/cores/esp8266/Esp.h
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

    return json;
  }
}
