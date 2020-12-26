#pragma once
#include "flightStrip.h"
#include <string>
#include <vector>

namespace plugInSettings {
	void loadSettings();
	std::vector<stripType> getTypes();
}