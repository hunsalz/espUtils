#include "Driver.h"

Driver::Driver(bool enabled) {
	this->enabled = enabled;
}

Driver::~Driver() {
	enabled = false;
}

bool Driver::isEnabled() {
	return enabled;
}

void Driver::setEnabled(bool enabled) {
	this->enabled = enabled;
}
