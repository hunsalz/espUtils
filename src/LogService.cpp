#include "LogService.h"

namespace esp8266util {

  LogService::LogService(String path) {
    this->path = path;
  }

  LogService::~LogService() {
    getFile().close();
  }

  String LogService::getPath() {
    return path;
  }

  int LogService::getOffset() {

    // read offset from file if not set before (is null)
    if (!offset) {
      file.seek(0, SeekSet);
      String line = file.readStringUntil('\n');
      offset = new uint16_t;
      *offset = line.toInt();
      Log.verbose(F("Offset from file is [%d]." CR), *offset);
    }
    // if limit of max bytes reached, then rollover and override entries from beginning
    if (*offset > getMaxBytes()) {
      *offset = INITIAL_OFFSET;
    }

    return *offset;
  }

  uint16_t LogService::getMaxBytes() {
    return maxBytes;
  }

  void LogService::setMaxBytes(uint16_t maxBytes) {
    this->maxBytes = maxBytes;
  }

  void LogService::write(char* buffer) {

    File file = getFile();
    if (file) {
      Log.verbose(F("Write log entry :: %s" CR), buffer);
      file.seek(getOffset(), SeekSet);
      file.printf(buffer);
      *offset = *offset + strlen(buffer);
      writeOffset();
    }
  }

  void LogService::writeOffset() {

    file.seek(0, SeekSet);
    char digits[10]; // 10 digits
    sprintf(digits, "%-10d", *offset);
    Log.verbose(F("Write offset [%s]" CR), digits);
    file.printf("%s\n", digits);
    file.flush();
  }

  File LogService::getFile() {

    if (!file) {
      if (SPIFFS.exists(path)) {
        file = SPIFFS.open(path, "r+");
        if (file) {
          Log.verbose(F("Open file [%s] successful." CR), path.c_str());
        } else {
          Log.error(F("Open file [%s] failed." CR), path.c_str());
        }
      } else {
        Log.verbose(F("Creating file [%s] ..." CR), path.c_str());
        file = SPIFFS.open(path, "w+");
        if (file) {
          *offset = INITIAL_OFFSET;
          writeOffset();
          Log.error(F("Creating file [%s] successful." CR), path.c_str());
        } else {
          Log.error(F("Creating file [%s] failed." CR), path.c_str());
        }
      }
    }

    return file;
  }
}
