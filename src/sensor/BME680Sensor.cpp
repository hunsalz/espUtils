#include "BME680Sensor.h"

namespace esp8266utils {

bool BME680Sensor::begin(uint8_t i2cAddr, const char *device) {
  
  _device = device;
  _bme680 = new Adafruit_BME680();
  if (_bme680->begin(i2cAddr)) {
    // set up oversampling and filter initialization
    _bme680->setTemperatureOversampling(BME680_OS_8X);
    _bme680->setHumidityOversampling(BME680_OS_2X);
    _bme680->setPressureOversampling(BME680_OS_4X);
    _bme680->setIIRFilterSize(BME680_FILTER_SIZE_3);
    _bme680->setGasHeater(320, 150); // 320*C for 150 ms
    return true;
  } else {
    return false;
  }
}

Adafruit_BME680 &BME680Sensor::getBME680() {
  return *_bme680;
}

bool BME680Sensor::update(bool mock) {
  
  if (_bme680 && !mock) {
    if (!_bme680->performReading()) {
      ERROR_MSG_P(F("Perform reading BMP680 values failed."));
      return false;
    } else {
      _temperature = _bme680->readTemperature();  // unit is Celsius, °C
      _humidity = _bme680->readHumidity();  // unit in percent, %
      _pressure = _bme680->readPressure();  // unit is Pascal (Pa) - // https://en.wikipedia.org/wiki/Pascal_(unit)
      _gas = _bme680->readGas(); // unit is Ohm, Ω
      _altitude = _bme680->readAltitude(1013.25);  // use standard baseline - // https://en.wikipedia.org/wiki/Pressure_altitude
      return true;
    }
  } else {
    _temperature = random(180, 310) / 10.0;
    _humidity = random(50, 150) / 10.0;
    _pressure = random(10000, 12000);
    _gas = random(5000, 12000);
    _altitude = random(100, 120);
    return true;
  }
}

float BME680Sensor::getTemperature() {
  return _temperature;
}

float BME680Sensor::getHumidity() {
  return _humidity;
}

float BME680Sensor::getPressure() {
  return _pressure;
}

uint32_t BME680Sensor::getGasResistance() {
  return _gas;
}

float BME680Sensor::getApproximateAltitude() {
  return _altitude;
}

const char *BME680Sensor::getDevice() {
  return _device;
}

String BME680Sensor::getValuesAsJson() {
  
  DynamicJsonDocument doc;
  JsonObject object = doc.to<JsonObject>();
  object["temperature"] = getTemperature();
  object["humidity"] = getHumidity();
  object["pressure"] = getPressure();
  object["gas"] = getGasResistance();
  object["altitude"] = getApproximateAltitude();
  object["device"] = getDevice();
  
  return esp8266utils::toString(object);
}
}  // namespace esp8266utils
