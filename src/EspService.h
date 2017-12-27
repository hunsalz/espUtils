#ifndef ESPSERVICE_H
#define ESPSERVICE_H

#ifdef ESP8266
extern "C" {
  #include "user_interface.h" // Expressif ESP8266 Non-OS API
}
#endif

#include <ESP.h> // https://github.com/esp8266/Arduino/blob/master/cores/esp8266/Esp.h
#include <Logger.h> // https://github.com/hunsalz/log4arduino
#include <ArduinoJson.h> // https://github.com/bblanchon/ArduinoJson

#include "Service.h"

using log4arduino::LOG;

namespace esp8266util {

  class EspService : public Service {

    public:

      bool available();

      EspClass& getESP();

      long unsigned getRemainingLoopInterval();

      bool nextLoopInterval();

      int getLoopInterval();

      void setLoopInterval(int milliseconds);

      int getDeepSleepInterval();

      void setDeepSleepInterval(int milliseconds);

      JsonObject& getDetails();

    private:

      int _loopInterval = 5000; // milliseconds
      unsigned long _lastLoopInterval = millis();
      int _deepSleepInterval = 60000; // milliseconds
  };

  extern EspService SYSTEM;
}

#endif // ESPSERVICE_H