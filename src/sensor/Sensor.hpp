#pragma once

#include <Arduino.h>  // https://github.com/esp8266/Arduino/blob/master/cores/esp8266/Arduino.h
#include <ESP.h>  // https://github.com/esp8266/Arduino/blob/master/cores/esp8266/Esp.h

namespace esp8266utils {

class Sensor {
 
 public:
  String getDeviceName() {
    return "ESP-" + String(ESP.getChipId());
  }
};
}  // namespace esp8266utils