#pragma once

#include <FS.h> // https://github.com/esp8266/Arduino/tree/master/cores/esp8266/FS.h
#include <ArduinoLog.h> // https://github.com/thijse/Arduino-Log
#include <ArduinoJson.h> // https://github.com/bblanchon/ArduinoJson

#include "Service.h"

namespace esp8266util {

  class FSService : public Service {

    public:

      ~FSService();

      bool isRunning();

      bool start();

      bool stop();

      FS* getFS();

      JsonObject& getStorageDetails();

      JsonArray& getFileListing();

      static String formatBytes(size_t bytes);

      static FSService getInstance();

    private:

      FSService();

      bool running = false;
  };

  extern FSService FILESYSTEM;
}
