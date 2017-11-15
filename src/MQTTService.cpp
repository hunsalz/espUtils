#include "MQTTService.h"

namespace esp8266util {

  MQTTService::~MQTTService() {
    end();
  }

  bool MQTTService::isSetup() {
    return _setupDone;
  }

  bool MQTTService::isRunning() {
    return getMqttClient().connected();
  }

  bool MQTTService::begin() {

    if (isSetup()) {
      if (!isRunning()) {
        // add verbose callback handlers
        _mqttClient.onConnect([this](bool sessionPresent) {
          Log.verbose(F("Connected to MQTT broker [%s:%d]" CR), getHostName(), getPort());
        });
        _mqttClient.onDisconnect([this](AsyncMqttClientDisconnectReason reason) {
          Log.verbose(F("Disconnected from MQTT broker [%s:%d]" CR), getHostName(), getPort());
        });
        _mqttClient.onSubscribe([](uint16_t packetId, uint8_t qos) {
          Log.verbose(F("Subscribe acknowledged with packetId [%d]" CR), packetId);
        });
        _mqttClient.onUnsubscribe([](uint16_t packetId) {
          Log.verbose(F("Unsubscribe acknowledged with packetId [%d]" CR), packetId);
        });
        _mqttClient.onMessage([](char* topic, char* payload, AsyncMqttClientMessageProperties properties, size_t len, size_t index, size_t total) {
          Log.verbose(F("Message received for topic [%s]: %s" CR), topic, payload);
        });
        _mqttClient.onPublish([](uint16_t packetId) {
          Log.verbose(F("Publish acknowledged with packetId [%d]" CR), packetId);
        });
        // configure mqtt client
        _mqttClient.setServer(getHostName(), getPort());

    // TODO

        // mqttClient.setCredentials(MQTT_USERNAME, MQTT_PASSWD);
        // mqttClient.setClientId(MQTT_CLIENT);
        // mqttClient.setKeepAlive(15); // in seconds
        // mqttClient.setCleanSession(true);
        // try to connect
        _mqttClient.connect();
      }
    } else {
      Log.error("Call setup() first.");
    }

    return isRunning();
  }

  bool MQTTService::end() {

    if (isRunning()) {
      _mqttClient.disconnect();
    }

    return isRunning();
  }

  AsyncMqttClient& MQTTService::getMqttClient() {
    return _mqttClient;
  }

  bool MQTTService::setup(const char* hostName, uint16_t port) {

    _hostName = hostName;
    _port = port;

    _setupDone = true;

    return isSetup();
  }

  const char* MQTTService::getHostName() {
    return _hostName;
  }

  uint16_t MQTTService::getPort() {
    return _port;
  }

  void MQTTService::publish(const char* topic, JsonObject& json) {

    // convert JSON into char array payload
    int length = json.measureLength() + 1;
    char payload[length];
    json.printTo(payload, length);

    // TODO check publish
    _mqttClient.publish(topic, 0, true, payload, length);
    Log.verbose(F("Message send: %s" CR), payload);
  }
}
