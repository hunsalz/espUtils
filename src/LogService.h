#pragma once

#include <FS.h> // https://github.com/esp8266/Arduino/tree/master/cores/esp8266/FS.h
#include <ArduinoLog.h> // https://github.com/thijse/Arduino-Log

namespace esp8266util {

  class LogService {

    public:

      LogService(String path);
      ~LogService();

      String getPath();

      int getOffset();

      uint16_t getMaxBytes();

      void setMaxBytes(uint16_t maxBytes);

      void write(char* buffer);

    private:

      String path;
      File file;
      uint16_t* offset = NULL; // undefined by default
      uint16_t maxBytes = 128; // default size 4KBytes

      const static int INITIAL_OFFSET = 11; // 10 digits and CR

      void writeOffset();

      File getFile();
  };
}
