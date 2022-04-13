#pragma once
#include "flightStrip.h"
#include <iostream>
#include <fstream>
#include <string>
#include <vector>

namespace plugInSettings {
	class settings {
	protected:
		std::ifstream settingsfStream;
		bool open();
		bool parseStrips();
		bool nextLine();
		int resolveType(std::string keyword2);
		std::ptrdiff_t addIfNew(std::string setname);
		bool setExists(std::string setname);
		void logParseError(std::string message);
		std::ptrdiff_t findIndex(std::string setname);
		std::vector<std::string> splitFields();
		std::string getDllPath();
		std::string currentLine{};
		std::vector<stripSet> stripSets;
		std::string m_dllPath;
		int lineNumber;
		bool parseStatus;
	public:
		settings();
		bool load();
		bool changeCurrentStripSet(std::string setName);
		std::string customICAOLocation;
		stripSet currentStripSet;
		stripSet getStripSet(std::string setName);
		std::vector<stripSet> getStripSets();
		std::string dllPath();
		std::string printerIP{"0.0.0.0"};
	};
}

extern plugInSettings::settings settings;