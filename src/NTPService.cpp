#include "NTPService.h"

namespace esp8266util {

  NTPService::NTPService(const char* poolServerName, int timeOffset, int updateInterval) :
    ntpClient(ntpUDP, poolServerName, timeOffset, updateInterval) {

    this->poolServerName = poolServerName;
    this->timeOffset = timeOffset;
    this->updateInterval = updateInterval;
  }

  NTPService::~NTPService() {
    stop();
  }

  bool NTPService::isSetup() {
    return true;
  }

  bool NTPService::isRunning() {
    return running;
  }

  bool NTPService::start() {

    if (!isRunning()) {
      ntpClient.begin();
      running = ntpClient.forceUpdate();
      if (running) {
        Log.verbose(F("Connection to NTP server [%s] established." CR), getPoolServerName());
      } else {
        Log.error(F("Connection to NTP server [%s] failed." CR), getPoolServerName());
      }
    }

    return isRunning();
  }

  bool NTPService::stop() {

    if (isRunning()) {
      ntpClient.end();
      running = false;
    }

    return isRunning();
  }

  NTPClient NTPService::getNTPClient() {

    if (!ntpClient.update()) {
      Log.error(F("Updating time with NTP server [%s] failed." CR), getPoolServerName());
    }

    return ntpClient;
  }

  const char* NTPService::getPoolServerName() {
    return poolServerName;
  }

  int NTPService::getTimeOffset() {
    return timeOffset;
  }

  int NTPService::getUpdateInterval() {
    return updateInterval;
  }

  JsonObject& NTPService::getDetails() {

    DynamicJsonBuffer jsonBuffer;
    JsonObject &json = jsonBuffer.createObject();
    json[F("ntpServer")] = getPoolServerName();
    json[F("UTCOffset")] = getTimeOffset();
    json[F("updateInterval")] = getUpdateInterval();
    json[F("time")] = getNTPClient().getFormattedTime();
    json[F("timestamp")] = getNTPClient().getEpochTime();

    return json;
  }
}
