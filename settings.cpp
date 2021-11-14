#include "pch.h"
#include "settings.h"
#include "constant.h"
#include "StripMaker.h"
#include "logger.h"
#include <sstream>
#include <winuser.h>
#include <algorithm>

plugInSettings::settings settings;

namespace plugInSettings {

	bool settings::load() {
		m_dllPath = getDllPath();
		if (!open()) {
			MessageBox(GetActiveWindow(), "Could not load strip sets. Quitting.", "StripMaker", MB_ICONERROR);
			EuroScopePlugInExit();
		}
		else {
			if (!parseStrips()) {
				MessageBox(GetActiveWindow(), "Error parsing strip sets. Quitting.", "StripMaker", MB_ICONERROR);
				EuroScopePlugInExit();
			}
		}
		return true;
	}
	bool settings::parseStrips() {
		while (nextLine()) {
			std::vector<std::string> fields = splitFields();
			if (fields.size() < 3) {
				//TODO: Log this error
				continue;
			}
			std::string setName = fields.at(0);
			std::string keyword = fields.at(1);
			auto setIndex = addIfNew(setName);
			if (keyword == "DESC"){ // set setName description
				stripSets.at(setIndex).setDescription = fields.at(2);
			}
			else { // we should have a stripType here, we'll parse it
				int stripType;
				if (keyword == "DEP") { stripType = TYPE_DEPARTURE; } else
				if (keyword == "ARR") { stripType = TYPE_ARRIVAL; } else
				if (keyword == "OVR") { stripType = TYPE_OVERFLIGHT; } else
				if (keyword == "ENR") { stripType = TYPE_ENROUTE; }
				else { // the stripType doesn't exist
					//TODO: log error
					continue;
				}
				std::string keyword2 = fields.at(2);
				if (keyword2 == "TEMPLATE") {
					if (!(fields.size() == 4)) {
						// TODO: log error
						continue;
					}
					else {
						if (!std::ifstream{ dllPath().append("\\templates\\").append(fields.at(3)) }) {
							// TODO: Log error
							continue;
						}
						else {
							stripSets.at(setIndex).type[stripType].templateFile = fields.at(3);
						}
					}
				}
				else if (fields.size() == 6) {
					int field = resolveType(keyword2);
					if (field == -1) { // field not found
						// TODO: Log error
						continue;
					}
					else {
						stripSets.at(setIndex).type[stripType].fields[field].fieldXLocation = std::stoi(fields.at(3));
						stripSets.at(setIndex).type[stripType].fields[field].fieldYLocation = std::stoi(fields.at(4));
						stripSets.at(setIndex).type[stripType].fields[field].fieldHeight = std::stoi(fields.at(5));

					}
				}
				else { // Not enough arguments after stripField
					// TODO: Log error
					continue;
				}
			}
		}
		// TODO: Sanity-check the settings we ended up with
		currentStripSet = getStripSet("UK");
		return true;
	}
	int settings::resolveType(std::string keyword2) { // this should really be an enum!
		int field;
		if (keyword2 == "CALLSIGN") {
			field = FIELD_CALLSIGN;
		}
		else if (keyword2 == "PHONETIC") {
			field = FIELD_PHONETIC_CALLSIGN;
		}
		else if (keyword2 == "ACTYPE") {
			field = FIELD_AIRCRAFT_TYPE;
		}
		else if (keyword2 == "SQUAWK") {
			field = FIELD_SQUAWK;
		}
		else if (keyword2 == "TAS") {
			field = FIELD_TAS;
		}
		else if (keyword2 == "RFL") {
			field = FIELD_RFL;
		}
		else if (keyword2 == "RULES") {
			field = FIELD_RULES;
		}
		else if (keyword2 == "SLOT") {
			field = FIELD_SLOT;
		}
		else if (keyword2 == "EDT") {
			field = FIELD_EDT;
		}
		else if (keyword2 == "ADEP") {
			field = FIELD_ADEP;
		}
		else if (keyword2 == "ROUTE") {
			field = FIELD_ROUTE;
		}
		else if (keyword2 == "ADES") {
			field = FIELD_ADES;
		}
		else if (keyword2 == "ADEPROUTE") {
			field = FIELD_ADEPROUTE;
		}
		else {
			return -1; // invalid field
		}
		return field;
	}
	std::ptrdiff_t settings::findIndex(std::string setName) {
		std::ptrdiff_t index;
		index = std::find_if(stripSets.begin(), stripSets.end(), 
			[setName](stripSet set) -> bool {
			return set.setName == setName; 
			}) - stripSets.begin();
		return index;
	}
	std::ptrdiff_t settings::addIfNew(std::string setName) {
		if (!setExists(setName)) {
			stripSets.push_back(stripSet{ setName });
		}
		return findIndex(setName);
	}
	bool settings::setExists(std::string setName) {
		return !(std::find_if(stripSets.begin(), stripSets.end(), 
			[setName](stripSet set) -> bool {
			return set.setName == setName;
			}) == stripSets.end());
	}
	bool settings::open() {
		settingsfStream = std::ifstream{ dllPath().append("stripsets.txt") };
		if (!settingsfStream) {
			return false;
		}
		else {
			return true;
		}
	}
	bool settings::nextLine() {
		if (settingsfStream) {
			std::getline(settingsfStream, currentLine);
			return true;
		}
		return false;
	}
	stripSet settings::getStripSet(std::string setName) {
		// TODO: like some sort of error checking as it will otherwise CRASH AND BURN
		return stripSets.at(findIndex(setName));
	}
	std::vector<std::string> settings::splitFields() {
		std::stringstream currentStream(currentLine);
		std::string field;
		std::vector<std::string> fields;

		while (std::getline(currentStream,field,':')) {
			fields.push_back(field);
		}
		return fields;
	}
	std::string settings::getDllPath() {
		char DllPathFile[_MAX_PATH];
		GetModuleFileNameA(HINSTANCE(&__ImageBase), DllPathFile, sizeof(DllPathFile));
		std::string path = DllPathFile;
		path.resize(path.size() - strlen("StripMaker.dll"));
		return path;
	}
	std::string settings::dllPath() {
		return m_dllPath;
	}
}