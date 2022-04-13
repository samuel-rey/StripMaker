#include "pch.h"
#include "flightStrip.h"
#include "constant.h"
#include "settings.h"
#include <algorithm>
#include <exception>

using namespace cimg_library;

flightStrip::flightStrip(int type, std::vector<std::string> fpContents) { // constructor. sets strip type and fills stripContents with those provided
	if (settings.currentStripSet.type[type].templateFile == "") {
		std::string message = std::string("Cannot create strip as strip type ").append(typeToString(type)).append(" does not have a template file specified in the settings.");
		MessageBox(GetActiveWindow(), message.c_str(), NULL, MB_OK | MB_ICONERROR);
		throw std::exception();
	}
	if (settings.currentStripSet.type[type].layoutFile == "") {
		std::string message = std::string("Cannot create strip as strip type ").append(typeToString(type)).append(" does not have a layout file specified in the settings.");
		MessageBox(GetActiveWindow(), message.c_str(), NULL, MB_OK | MB_ICONERROR);
		throw std::exception();
	}
	else {
		stripLayout = settings.currentStripSet.type[type].layoutFile;
	}
	
	try {
		CImg <unsigned int>newTemplate(settings.dllPath().append("\\templates\\").append(settings.currentStripSet.type[type].templateFile).c_str());	
		stripTemplate = newTemplate;
	}
	catch (CImgIOException) {
		std::string message = std::string("Failed to load strip template BMP for strip type ").append(typeToString(type)).append(" in ").append(settings.dllPath().append(settings.currentStripSet.type[type].templateFile).c_str()).append("\nThe plugin will now close.");
		MessageBox(GetActiveWindow(), message.c_str(), NULL, MB_OK | MB_ICONERROR);
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

std::string typeToString(int type){
	switch (type) {
	case TYPE_DEPARTURE:
		return "Departure";
	case TYPE_ENROUTE:
		return "Enroute";
	case TYPE_ARRIVAL:
		return "Arrival";
	case TYPE_LOCAL:
		return "Local";
	default:
		return "Unknown";
	}
}
	
int stringToType(std::string typeName) {
	if (typeName == "Departure") {
		return TYPE_DEPARTURE;
	}
	else if (typeName == "Enroute") {
		return TYPE_ENROUTE;
	}
	else if (typeName == "Arrival") {
		return TYPE_ARRIVAL;
	}
	else if (typeName == "Local") {
		return TYPE_LOCAL;
	}
	else {
		return 0;
	}
}
