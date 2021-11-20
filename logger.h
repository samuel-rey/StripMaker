// "logger.h" was developed by the vSMR contributors, Pierre Ferran, Even Rognlien, Lionel Bischof, Daniel Lange, Juha Holopainen, Keanu Czirjak. Used under a GPL v3 license.
#pragma once
#include "pch.h"
#include "settings.h"
#include <string>
#include <sstream>
#include <sstream>
#include <iomanip>
#include <fstream>

class Logger {
public:
	static bool ENABLED;

	static void info(std::string message) {
		if (Logger::ENABLED && settings.dllPath().length() > 0) {
			std::ofstream file;
			file.open(settings.dllPath() + "\\logs\\stripmaker.log", std::ofstream::out | std::ofstream::app);
			file << "INFO: " << message << std::endl;
			file.close();
		}
	}
	static void error(std::string message) {
		if (Logger::ENABLED && settings.dllPath().length() > 0) {
			std::ofstream file;
			file.open(settings.dllPath() + "\\logs\\stripmaker.log", std::ofstream::out | std::ofstream::app);
			file << "ERROR: " << message << std::endl;
			file.close();
		}
	}
};