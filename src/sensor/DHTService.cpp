#include "DHTService.h"

namespace esp8266util {

  DHTService::DHTService() {
    // expects late initialization
  };

  DHTService::DHTService(uint8_t pin, Type type) {
    init(pin, type);
  };

  void DHTService::init(uint8_t pin, Type type) {
    dht = new DHT_Unified(pin, type);
  }

  void init(JsonObject& json) {
    
  }

  float DHTService::getTemperature() {

    float value = -1;
    if (dht) {
      sensors_event_t event;
      dht->temperature().getEvent(&event);
      if (isnan(event.temperature)) {
        Log.error("Error reading temperature");
      } else {
        value = event.temperature;
      }
    }

    return value;
  }

  float DHTService::getHumidity() {

    float value = -1;
    if (dht) {
      sensors_event_t event;
      dht->humidity().getEvent(&event);
      if (isnan(event.relative_humidity)) {
        Log.error("Error reading temperature");
      } else {
        value = event.relative_humidity;
      }
    }

    return value;
  }

  JsonArray& DHTService::getDetails() {

    sensor_t sensor;
    DynamicJsonBuffer jsonBuffer;
    JsonArray& json = jsonBuffer.createArray();
    if (dht) {
      // map temperature sensor values
      dht->temperature().getSensor(&sensor);
      JsonObject& temperature = json.createNestedObject().createNestedObject(F("temperature"));
      temperature[F("name")] = sensor.name;
      temperature[F("version")] = sensor.version;
      temperature[F("id")] = sensor.sensor_id;
      temperature[F("min")] = sensor.max_value;
      temperature[F("max")] = sensor.min_value;
      temperature[F("resolution")] = sensor.resolution;
      // map humidity sensor values
      dht->humidity().getSensor(&sensor);
      JsonObject& humidity = json.createNestedObject().createNestedObject(F("humidity"));
      humidity[F("name")] = sensor.name;
      humidity[F("version")] = sensor.version;
      humidity[F("id")] = sensor.sensor_id;
      humidity[F("min")] = sensor.max_value;
      humidity[F("max")] = sensor.min_value;
      humidity[F("resolution")] = sensor.resolution;
    }

    return json;
  }
}
