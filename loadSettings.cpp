#include "pch.h"
#include "loadSettings.h"
#include "constant.h"
#include "dllpath.h"
//#include "logger.h"

// loads stripTypes defined in settings file and other settings such as printer into. Not yet implemented. For now, loads hard-wired settings (just 1 strip type: UKDeparture)

std::vector<stripType> loadedTypes;

namespace plugInSettings {
	void loadSettings() {
		findDllPath();
		loadedTypes.push_back(stripType());
		loadedTypes[0].stripName = "UKDeparture";
		try {
			loadedTypes[0].stripTemplate = CImg<unsigned char>(getDllPath().append("templates\\ukdeparture.bmp").c_str());
		}
		catch (CImgIOException) {
			std::string message = std::string("Failed to load strip template BMP for strip type ").append(loadedTypes[0].stripName).append(" in ").append(getDllPath().append("templates\\ukdeparture.bmp").c_str()).append("\nThe plugin will now close.");
			MessageBox(GetActiveWindow(), message.c_str(), NULL, MB_OK|MB_ICONERROR);
			EuroScopePlugInExit();
		}
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