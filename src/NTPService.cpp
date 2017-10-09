#include "NTPService.h"

namespace esp8266util {

  // TODO check complete class 

  NTPService::NTPService() :
    ntpClient(ntpUDP, "europe.pool.ntp.org", 3600, 60000) {}

  NTPService::~NTPService() {
    stop();
  }

  bool NTPService::isRunning() {
    return running;
  }

  bool NTPService::start() {

    if (!isRunning()) {
      ntpClient.begin();
      running = ntpClient.forceUpdate();
      if (running) {
        //Log.verbose(F("NTP established to [%s:%d]" CR), NTP_SERVER, NTP_DEFAULT_LOCAL_PORT);
      } else {
        //Log.error(F("NTP failed for [%s:%d]" CR), NTP_SERVER, NTP_DEFAULT_LOCAL_PORT);
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
      Log.error("Updating NTP failed.\n");

      // TODO implement retry and/or recovery mechanisms
    }

    return ntpClient;
  }
}
