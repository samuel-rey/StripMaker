#pragma once
#include "flightStrip.h"
#include <spdlog/logger.h>
#include <spdlog/sinks/basic_file_sink.h>
#include <string>
#include <vector>

std::vector<stripType> loadedTypes;
void loadSettings();