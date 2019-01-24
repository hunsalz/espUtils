#pragma once

#ifdef ESP8266
extern "C" {
#include "user_interface.h"  // Expressif ESP8266 Non-OS API
}
#endif

#include <ArduinoJson.h>  // https://github.com/bblanchon/ArduinoJson
#include <ESP.h>  // https://github.com/esp8266/Arduino/blob/master/cores/esp8266/Esp.h

namespace ESPUtils {

inline size_t serializeESP(String& output) {

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
    serializeJson(object, output);
    return measureJson(object);
  }

}  // namespace ESPUtils