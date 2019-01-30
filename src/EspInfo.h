#pragma once

#ifdef ESP8266
  extern "C" {
    #include "user_interface.h"  // Expressif ESP8266 Non-OS API
  }
#endif

#include <ArduinoJson.h>         // https://github.com/bblanchon/ArduinoJson
#include <ESP.h>                 // https://github.com/espressif/arduino-esp32/blob/master/cores/esp32/Esp.h
                                 // https://github.com/esp8266/Arduino/blob/master/cores/esp8266/Esp.h

namespace espUtils {

inline size_t serializeESP(String& output) {

    DynamicJsonDocument doc;
    JsonObject object = doc.to<JsonObject>();
    
    #ifdef ESP32
      // internal RAM
      object[F("heapSize")] = ESP.getHeapSize();
      object[F("freeHeap")] = ESP.getFreeHeap();
      object[F("minFreeHeap")] = ESP.getMinFreeHeap();
      object[F("maxAllocHeap")] = ESP.getMaxAllocHeap();

      // SPI RAM
      object[F("psramSize")] = ESP.getPsramSize();
      object[F("freePsram")] = ESP.getFreePsram();
      object[F("minFreePsram")] = ESP.getMinFreePsram();
      object[F("maxAllocPsram")] = ESP.getMaxAllocPsram();

      object[F("psramSize")] = ESP.getPsramSize();
      object[F("freePsram")] = ESP.getFreePsram();
      object[F("minFreePsram")] = ESP.getMinFreePsram();
      object[F("maxAllocPsram")] = ESP.getMaxAllocPsram();

      object[F("chipRevision")] = ESP.getChipRevision();
      object[F("cpuFreqMHz")] = ESP.getCpuFreqMHz();
      object[F("cycleCount")] = ESP.getCycleCount();
      object[F("sdkVersion")] = ESP.getSdkVersion();

      object[F("flashChipSize")] = ESP.getFlashChipSize();
      object[F("flashChipSpeed")] = ESP.getFlashChipSpeed();
      object[F("flashChipMode")] = ESP.getFlashChipMode();

      object[F("sketchSize")] = ESP.getSketchSize();
      object[F("sketchMD5")] = ESP.getSketchMD5();
      object[F("freeSketchSpace")] = ESP.getFreeSketchSpace();

      object[F("efuseMac")] = ESP.getEfuseMac();
      
      object[F("uptime")] = millis(); 
    #else
      object[F("vcc")] = ESP.getVcc();
      object[F("chipId")] = ESP.getChipId();

      object[F("freeHeap")] = ESP.getFreeHeap();
      object[F("maxFreeBlockSize")] = ESP.getMaxFreeBlockSize();
      object[F("heapFragmentation")] = ESP.getHeapFragmentation();

      object[F("freeContStack")] = ESP.getFreeContStack();

      object[F("sdkVersion")] = ESP.getSdkVersion();
      object[F("coreVersion")] = ESP.getCoreVersion();
      object[F("fullVersion")] = ESP.getFullVersion();

      object[F("bootVersion")] = ESP.getBootVersion();
      object[F("bootMode")] = ESP.getBootMode();

      object[F("cpuFreqMHz")] = ESP.getCpuFreqMHz();

      object[F("flashChipId")] = ESP.getFlashChipId();
      //object[F("flashChipVendorId")] = ESP.getFlashChipVendorId();

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
    #endif

    serializeJson(object, output);
    return measureJson(object);
  }
}  // namespace espUtils