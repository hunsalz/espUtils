#pragma once

class Driver {

	public:
		// enabled: enables/disables this driver
		Driver(bool enabled = false);
		~Driver();

		// use this method to know if this driver is enabled or not
		bool isEnabled();

		// enables/disables this driver
		void setEnabled(bool enabled);

	private:

		bool enabled;
};
