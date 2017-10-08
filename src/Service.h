#pragma once

namespace esp8266util {

	class Service {

		public:

			Service();
			~Service();

			// returns if this service is running
			bool isRunning();

			// starts this service and returns true if service is running
			virtual bool start() = 0;

	    // stops this service and returns true if service is stopped
	    virtual bool stop() = 0;

			// TODO remove running declaration
			bool running = false;
	};
}
