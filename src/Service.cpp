#include <Service.h>

Service::Service() {
}

Service::~Service() {
	_running = false;
}

// returns if this service is running
bool Service::isRunning() {
	return _running;
}
