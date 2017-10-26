#pragma once

#include <ArduinoLog.h> // https://github.com/thijse/Arduino-Log
#include <ArduinoJson.h> // https://github.com/bblanchon/ArduinoJson
#include <DHT.h> // https://github.com/adafruit/DHT-sensor-library
#include <DHT_U.h> // https://github.com/adafruit/DHT-sensor-library

/*
 Open issues:
  - persist calls per sensor in logfile
  - add NTP if available
  - configure by service call
 */
namespace esp8266util {

  class DHTService {

    public:

      enum Type {
        DHT_11 = 11,
        DHT_21 = 21,
        DHT_22 = 22
      };

      DHTService();

      DHTService(uint8_t pin, Type type);

      void init(uint8_t pin, Type type);

      void init(JsonObject& json);

      float getTemperature();

      float getHumidity();

      JsonArray& getDetails();

    private:

      uint8_t pin;
      DHT_Unified* dht;
  };
}
