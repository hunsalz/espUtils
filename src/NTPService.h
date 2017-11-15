#ifndef NTPSERVICE_H
#define NTPSERVICE_H

#include <ArduinoLog.h> // https://github.com/thijse/Arduino-Log
#include <ArduinoJson.h> // https://github.com/bblanchon/ArduinoJson
#include <NtpClientLib.h> // https://github.com/gmag11/NtpClient/blob/master/src/NtpClientLib.h

#include "Service.h"

namespace esp8266util {

  class NTPService : public Service {

    public:

      ~NTPService();

      bool available();

      bool begin(String ntpServerName = DEFAULT_NTP_SERVER, int timeOffset = DEFAULT_NTP_TIMEZONE, bool daylight = false);

      bool end();

      NTPClient& getNTPClient();

      JsonObject& getDetails();

    private:

      bool _available = false;
  };

  extern NTPService NTP_SERVICE;
}

#endif // NTPSERVICE_H