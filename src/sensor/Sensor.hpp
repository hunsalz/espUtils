#pragma once

#include <Arduino.h>  // https://github.com/esp8266/Arduino/blob/master/cores/esp8266/Arduino.h
#include <ESP.h>  // https://github.com/esp8266/Arduino/blob/master/cores/esp8266/Esp.h

namespace esp8266utils {

class Sensor {
 
 public:
  
  const char* getDeviceName() {

    String str = "ESP-" + String(ESP.getChipId());
    return str.c_str();
  }
};
}  // namespace esp8266utils