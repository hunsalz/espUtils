#pragma once

namespace esp8266util {

	class Service {

		public:

			// returns if this service is running
			virtual bool isRunning() = 0;

			// starts this service and returns true if service is running
			virtual bool start() = 0;

	    // stops this service and returns true if service is stopped
	    virtual bool stop() = 0;
	};
}
