#pragma once

#include <ESP.h>  // https://github.com/espressif/arduino-esp32/blob/master/cores/esp32/Esp.h
                  // https://github.com/esp8266/Arduino/blob/master/cores/esp8266/Esp.h

namespace espUtils {

class Sensor {

 public:

  static int getDevice(char* buffer) {
  
    #ifdef ESP32
      return sprintf(buffer, "ESP-%010x", (uint32_t)ESP.getEfuseMac()); // use low 4 bytes
    #else
      return sprintf(buffer, "ESP-%010x", ESP.getChipId());
    #endif
  }
};
}  // namespace espUtils