#include "pch.h"
#include "flightStrip.h"
#include "constant.h"
#include "loadSettings.h"
#include "dllpath.h"

flightStrip::flightStrip(stripType _Type, std::vector<std::string> fpContents) { // constructor. sets strip type and fills stripContents with those provided
	type = _Type;
	for (int i = 0; i < FIELDS_TOTAL; i++) {
		fieldContents.push_back(std::string());
		fieldContents[i] = fpContents[i];
	}
	applyTextToFields();
}

void flightStrip::applyTextToFields() { // takes the stripContents and writes them into the stripTemplate with the settings provided by the stripType
	for (int i = 0; i < FIELDS_TOTAL; i++) {
		const unsigned char black[] = { 0,0,0 };
		type.stripTemplate.draw_text(type.fields[i].fieldXLocation, type.fields[i].fieldYLocation, fieldContents[i].c_str(), black, 0, 1, type.fields[i].fieldHeight);
	}
}

void flightStrip::display() { // opens a window with the generated strip and saves it to disk for debug purposes
	try {
		type.stripTemplate.save(plugInSettings::getDllPath().append("strip.bmp").c_str());
	}
	catch (CImgIOException) {
		MessageBox(GetActiveWindow(), std::string("Failed to save strip to ").append(plugInSettings::getDllPath()).append("\\strip.bmp").c_str(),NULL,MB_OK|MB_ICONERROR);
	}
	CImgDisplay main_disp(type.stripTemplate, "Flight Strip",0);
	while (!main_disp.is_closed()) {
		main_disp.wait();
	}
}