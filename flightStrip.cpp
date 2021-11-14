#include "pch.h"
#include "flightStrip.h"
#include "constant.h"
#include "settings.h"
#include <algorithm>

flightStrip::flightStrip(int type, std::vector<std::string> fpContents) { // constructor. sets strip type and fills stripContents with those provided
	try {
		CImg <unsigned int>newTemplate(settings.dllPath().append("\\templates\\").append(settings.currentStripSet.type[type].templateFile).c_str());
		stripTemplate = newTemplate;
	}
	catch (CImgIOException) {
		std::string message = std::string("Failed to load strip template BMP for strip type ").append(settings.currentStripSet.setName).append(" in ").append(settings.dllPath().append("templates\\ukdeparture.bmp").c_str()).append("\nThe plugin will now close.");
		MessageBox(GetActiveWindow(), message.c_str(), NULL, MB_OK | MB_ICONERROR);
		EuroScopePlugInExit();
	}
	std::copy(std::begin(settings.currentStripSet.type[type].fields), std::end(settings.currentStripSet.type[type].fields), std::begin(fields));
	for (int i = 0; i < FIELDS_TOTAL; i++) {
		fieldContents.push_back(std::string());
		fieldContents[i] = fpContents[i];
	}
	applyTextToFields();
}

void flightStrip::applyTextToFields() { // takes the stripContents and writes them into the stripTemplate with the settings provided by the stripType
	for (int i = 0; i < FIELDS_TOTAL; i++) {
		const unsigned char black[] = { 0,0,0 };
		stripTemplate.draw_text(fields[i].fieldXLocation, fields[i].fieldYLocation, fieldContents[i].c_str(), black, 0, 1, fields[i].fieldHeight);
	}
}

void flightStrip::display() { // opens a window with the generated strip and saves it to disk for debug purposes
	try {
		stripTemplate.save(settings.dllPath().append("strip.bmp").c_str());
	}
	catch (CImgIOException) {
		MessageBox(GetActiveWindow(), std::string("Failed to save strip to ").append(settings.dllPath()).append("\\strip.bmp").c_str(),NULL,MB_OK|MB_ICONERROR);
	}
	CImgDisplay main_disp(stripTemplate, "Flight Strip",0);
	while (!main_disp.is_closed()) {
		main_disp.wait();
	}
}