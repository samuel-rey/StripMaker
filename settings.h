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
		std::ptrdiff_t findIndex(std::string setname);
		std::vector<std::string> splitFields();
		std::string getDllPath();
		std::string currentLine{};
		std::vector<stripSet> stripSets;
		std::string m_dllPath;
	public:
		bool load();
		std::string customICAOLocation;
		stripSet currentStripSet;
		stripSet getStripSet(std::string setName);
		std::string dllPath();
	};
}

extern plugInSettings::settings settings;