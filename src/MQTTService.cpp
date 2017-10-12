#include "MQTTService.h"

namespace esp8266util {

  MQTTService::~MQTTService() {
    stop();
  }

  bool MQTTService::isSetup() {
    return setupDone;
  }

  bool MQTTService::isRunning() {
    return getMqttClient()->connected();
  }

  bool MQTTService::start() {

    if (isSetup()) {
      if (!isRunning()) {
        // add verbose callback handlers
        mqttClient.onConnect([this](bool sessionPresent) {
          Log.verbose(F("Connected to MQTT broker [%s:%d]" CR), getHostName(), getPort());
        });
        mqttClient.onDisconnect([this](AsyncMqttClientDisconnectReason reason) {
          Log.verbose(F("Disconnected from MQTT broker [%s:%d]" CR), getHostName(), getPort());
        });
        mqttClient.onSubscribe([](uint16_t packetId, uint8_t qos) {
          Log.verbose(F("Subscribe acknowledged with packetId [%d]" CR), packetId);
        });
        mqttClient.onUnsubscribe([](uint16_t packetId) {
          Log.verbose(F("Unsubscribe acknowledged with packetId [%d]" CR), packetId);
        });
        mqttClient.onMessage([](char* topic, char* payload, AsyncMqttClientMessageProperties properties, size_t len, size_t index, size_t total) {
          Log.verbose(F("Message received for topic [%s]: %s" CR), topic, payload);
        });
        mqttClient.onPublish([](uint16_t packetId) {
          Log.verbose(F("Publish acknowledged with packetId [%d]" CR), packetId);
        });
        // configure mqtt client
        mqttClient.setServer(getHostName(), getPort());

    // TODO

        // mqttClient.setCredentials(MQTT_USERNAME, MQTT_PASSWD);
        // mqttClient.setClientId(MQTT_CLIENT);
        // mqttClient.setKeepAlive(15); // in seconds
        // mqttClient.setCleanSession(true);
        // try to connect
        mqttClient.connect();
      }
    } else {
      Log.error("Call setup() first.");
    }

    return isRunning();
  }

  bool MQTTService::stop() {

    if (isRunning()) {
      mqttClient.disconnect();
    }

    return isRunning();
  }

  AsyncMqttClient* MQTTService::getMqttClient() {
    return &mqttClient;
  }

  bool MQTTService::setup(const char* hostName, uint16_t port) {

    this->hostName = hostName;
    this->port = port;

    setupDone = true;

    return isSetup();
  }

  const char* MQTTService::getHostName() {
    return hostName;
  }

  uint16_t MQTTService::getPort() {
    return port;
  }




  void MQTTService::publish(const char* topic, JsonObject& json) {

    // convert JSON into char array payload
    int length = json.measureLength() + 1;
    char payload[length];
    json.printTo(payload, length);

    // TODO check publish
    mqttClient.publish(topic, 0, true, payload, length);
    Log.verbose(F("Message send: %s" CR), payload);
  }
}
