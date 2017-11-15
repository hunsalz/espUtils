#ifndef DHTSERVICE_H
#define DHTSERVICE_H

#include <ArduinoLog.h> // https://github.com/thijse/Arduino-Log
#include <ArduinoJson.h> // https://github.com/bblanchon/ArduinoJson
#include <DHT.h> // https://github.com/adafruit/DHT-sensor-library
#include <DHT_U.h> // https://github.com/adafruit/DHT-sensor-library

namespace esp8266util {

  // TODO Service
  class DHTService {

    public:

      struct config_t {
        uint8_t pin;
       /**
        Valid types: 
        DHT11 = 11
        DHT21 = 21
        DHT22 = 22 
        */
        uint8_t type;
      };

      void begin(config_t config);

      config_t getConfig();

      DHT_Unified& getDHT();

      float getTemperature();

      float getHumidity();

      JsonArray& getDetails();

    private:

      DHT_Unified* _dht = NULL;
      config_t _config;
  };
}

#endif // DHTSERVICE_H
