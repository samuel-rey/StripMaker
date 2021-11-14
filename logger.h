// "logger.h" was developed by the vSMR contributors, Pierre Ferran, Even Rognlien, Lionel Bischof, Daniel Lange, Juha Holopainen, Keanu Czirjak. Used under a GPL v3 license.
#pragma once
#include "pch.h"
#include <string>
#include <sstream>
#include <sstream>
#include <iomanip>
#include <fstream>

class Logger {
public:
	static bool ENABLED;
	static std::string DLL_PATH;

	static void info(std::string message) {
		if (Logger::ENABLED && Logger::DLL_PATH.length() > 0) {
			std::ofstream file;
			file.open(Logger::DLL_PATH + "StripMaker\\logs\\stripmaker.log", std::ofstream::out | std::ofstream::app);
			file << "INFO: " << message << std::endl;
			file.close();
		}
	}
	static void error(std::string message) {
		if (Logger::ENABLED && Logger::DLL_PATH.length() > 0) {
			std::ofstream file;
			file.open(Logger::DLL_PATH + "StripMaker\\logs\\stripmaker.log", std::ofstream::out | std::ofstream::app);
			file << "ERROR: " << message << std::endl;
			file.close();
		}
	}
};