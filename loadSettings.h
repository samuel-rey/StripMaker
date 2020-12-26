#pragma once
#include "flightStrip.h"
//#include <spdlog/logger.h>
#include <string>
#include <vector>

std::vector<stripType> loadSettings();
const std::vector<stripType> loadedTypes = loadSettings();