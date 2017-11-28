#ifndef BMP280SERVICE_H
#define BMP280SERVICE_H

#include <ArduinoLog.h> // https://github.com/thijse/Arduino-Log
#include <ArduinoJson.h> // https://github.com/bblanchon/ArduinoJson
#include <Adafruit_BMP280.h> // https://github.com/adafruit/Adafruit_BMP280_Library

#include "Configurable.h"

namespace esp8266util {

  // TODO Service
  class BMP280Service {

    public:

      bool begin();

      Adafruit_BMP280& getBMP280();

      bool update();

      float getTemperature(); // unit is Celsius,°C

      float getPressure(); // unit is Pascal, Pa

      float getAltitude(); // unit in Meter, m

    private:

      Adafruit_BMP280* _bmp280 = NULL;

      float _temperature = NAN;
      float _pressure = NAN;
      float _altitude = NAN;
  };
}

#endif // BMP280SERVICE_H
