#include "DHTService.h"

namespace esp8266util {

  void DHTService::begin(DHTService::config_t config) {
    
    _config = config;
    _dht = new DHT_Unified(config.pin, config.type);
  }

  DHTService::config_t DHTService::getConfig() {
    return _config;
  }

  DHT_Unified& DHTService::getDHT() {
    return *_dht;
  }

  float DHTService::getTemperature() {

    float value = NAN;
    if (_dht) {
      sensors_event_t event;
      _dht->temperature().getEvent(&event);
      if (isnan(event.temperature)) {
        Log.error(F("Error reading temperature" CR));
      } else {
        value = event.temperature;

        // TODO
        char out[10];
        dtostrf(value, 1, 2, out);
        Serial.printf("---------------------------------Temp :: %s\n", out);
  
      }
    }

    return value;
  }

  float DHTService::getHumidity() {

    float value = NAN;
    if (_dht) {
      sensors_event_t event;
      _dht->humidity().getEvent(&event);
      if (isnan(event.relative_humidity)) {
        Log.error(F("Error reading humidity" CR));
      } else {
        value = event.relative_humidity;
      }
    }

    return value;
  }

  JsonArray& DHTService::getDetails() {

    DynamicJsonBuffer jsonBuffer;
    JsonArray& json = jsonBuffer.createArray();
    if (_dht) {
      sensor_t sensor;
      // map temperature sensor values
      _dht->temperature().getSensor(&sensor);
      JsonObject& temperature = json.createNestedObject().createNestedObject(F("temperature"));
      temperature[F("name")] = String(sensor.name);
      temperature[F("version")] = sensor.version;
      temperature[F("id")] = sensor.sensor_id;
      temperature[F("min")] = sensor.max_value;
      temperature[F("max")] = sensor.min_value;
      temperature[F("resolution")] = sensor.resolution;
      // map humidity sensor values
      _dht->humidity().getSensor(&sensor);
      JsonObject& humidity = json.createNestedObject().createNestedObject(F("humidity"));
      humidity[F("name")] = String(sensor.name);
      humidity[F("version")] = sensor.version;
      humidity[F("id")] = sensor.sensor_id;
      humidity[F("min")] = sensor.max_value;
      humidity[F("max")] = sensor.min_value;
      humidity[F("resolution")] = sensor.resolution;
    }

    return json;
  }
}
