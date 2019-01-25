#pragma once

#include <Arduino.h>      // https://github.com/espressif/arduino-esp32/blob/master/cores/esp32/Arduino.h
                          // https://github.com/esp8266/Arduino/blob/master/cores/esp8266/Arduino.h

#include <actuator/MotorDriver.hpp>

#include <polyfills/ContentType.h>

#include <sensor/BME280Sensor.hpp>
#include <sensor/BME680Sensor.hpp>
#include <sensor/BMP085Sensor.hpp>
#include <sensor/BMP280Sensor.hpp>
#include <sensor/DHTSensor.hpp>
#include <sensor/MQ135Sensor.hpp>
#include <sensor/Sensor.hpp>

#include <EspInfo.h>
#ifdef ESP8266
  #include <ESP8266FS.hpp>
#endif
#include <Logging.hpp>
#include <WiFiUtil.h>

#ifdef USE_ESP_ASYNC
  #include <ESPAsyncWebService.hpp>
  #include <ESPAsyncWebSocketListener.hpp>
#else
  #include <WebSocketsServerListener.hpp>
#endif