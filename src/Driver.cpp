#include "Driver.h"

Driver::Driver(bool enabled) {
	_enabled = enabled;
}

Driver::~Driver() {
	_enabled = false;
}

bool Driver::isEnabled() {
	return _enabled;
}

void Driver::setEnabled(bool enabled) {
	_enabled = enabled;
}
