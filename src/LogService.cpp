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

  uint16_t LogService::getOffset() {

    // if limit of max bytes is reached, then rollover and override entries from beginning
    if (offset > getMaxBytes()) {
      offset = INITIAL_OFFSET;
    }

    return offset;
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
      file.seek(getOffset(), SeekSet);
      file.printf(buffer);
      writeOffset(offset + strlen(buffer));
    }
  }

  void LogService::write(String str) {
    write(str.c_str());
  }

  StreamString LogService::getLog() {

    StreamString stream;
    String line;
    uint16_t offset = readOffset();
    // start reading from offset position until lines reached
    file.seek(offset, SeekSet);
    Log.verbose(F("Read file from offset [%d]." CR), offset);
    while (file.available()) {
      line = file.readStringUntil('\n');
      stream.println(line);
    }
    // continue reading from inital offset until lines reached or offset reached
    file.seek(INITIAL_OFFSET, SeekSet);
    int bytes = INITIAL_OFFSET;
    while (file.available() && bytes < offset) {
      line = file.readStringUntil('\n');
      bytes += line.length() + 1;
      stream.println(line);
    }
    stream.flush();

    return stream;
  }

  uint16_t LogService::readOffset() {

    int offset;
    File file = getFile();
    if (file) {
      file.seek(0, SeekSet);
      String line = file.readStringUntil('\n');
      offset = line.toInt();
      if (offset == 0) {
        Log.error(F("File [%s] doesn't contain a valid offset value." CR), path.c_str());
      } else {
        Log.verbose(F("Offset from file is [%d]." CR), offset);
      }
    }

    return offset;
  }

  void LogService::writeOffset(uint16_t offset) {

    file.seek(0, SeekSet);
    char digits[10]; // 10 digits
    sprintf(digits, "%-10d", offset);
    file.printf("%s\n", digits);
    file.flush();
    this->offset = offset;
  }

  File LogService::getFile() {

    if (!file) {
      if (SPIFFS.exists(path)) {
        file = SPIFFS.open(path, "r+");
        if (file) {
          Log.verbose(F("Open file [%s] successful." CR), path.c_str());
          offset = readOffset();
        } else {
          Log.error(F("Open file [%s] failed." CR), path.c_str());
        }
      } else {
        Log.verbose(F("Creating file [%s] ..." CR), path.c_str());
        file = SPIFFS.open(path, "w+");
        if (file) {
          Log.error(F("Creating new file [%s] successful." CR), path.c_str());
          writeOffset(INITIAL_OFFSET);
        } else {
          Log.error(F("Creating new file [%s] failed." CR), path.c_str());
        }
      }
    }

    return file;
  }
}
