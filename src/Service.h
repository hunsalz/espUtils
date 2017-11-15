#ifndef SERVICE_H
#define SERVICE_H

namespace esp8266util {

  class Service {

    public:

      // TODO rename to available()
      // returns if this service is running
      virtual bool isRunning() = 0;
  };
}

#endif // SERVICE_H