#ifndef SERVICE_H
#define SERVICE_H

namespace esp8266util {

  class Service {

    public:

      // returns if this service is available
      virtual bool available() = 0;
  };
}

#endif // SERVICE_H