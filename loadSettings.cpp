#include "pch.h"
#include "loadSettings.h"
//#include "logger.h"

// loads stripTypes defined in settings file and other settings such as printer into. Not yet implemented. For now, loads hard-wired settings (just 1 strip type: UKDeparture)

std::vector<stripType> loadedTypes;

namespace plugInSettings {
	void loadSettings() {
		loadedTypes.push_back(stripType());
		loadedTypes[0].stripName = "UKDeparture";
		loadedTypes[0].stripTemplate = CImg<unsigned char>("StripMaker/templates/ukdeparture.bmp");
		int xlocation[] = { 293,293,397,309,415,191,191,1 ,1  ,0,0,583,471 };
		int ylocation[] = { 68 ,45 ,1  ,114,148,62 ,92 ,1 ,121,0,0 ,157,95 };
		int fieldHeight[] = { 44 ,22 ,22 ,22 ,22 ,22 ,22 ,22,22 ,0,0 ,22,22 };
		for (int i = 0; i < 13; i++) {
			loadedTypes[0].fields.push_back(stripField());
			loadedTypes[0].fields[i].fieldXLocation = xlocation[i];
			loadedTypes[0].fields[i].fieldYLocation = ylocation[i];
			loadedTypes[0].fields[i].fieldHeight = fieldHeight[i];
		}
	}
	std::vector<stripType> getTypes() {
		return loadedTypes;
	}
}