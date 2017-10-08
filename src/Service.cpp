#include <Service.h>

namespace esp8266util {

	Service::Service() {
	}

	Service::~Service() {
		running = false;
	}

	// returns if this service is running
	bool Service::isRunning() {
		return running;
	}
}
