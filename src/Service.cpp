#include <Service.h>

Service::Service() {
}

Service::~Service() {
	running = false;
}

// returns if this service is running
bool Service::isRunning() {
	return running;
}
