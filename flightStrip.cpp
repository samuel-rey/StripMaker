#include "pch.h"
#include "flightStrip.h"
#include "constant.h"
#include "loadSettings.h"

flightStrip::flightStrip(stripType _Type, std::vector<std::string> fpContents) {
	type = _Type;
	for (int i = 0; i < FIELDS_TOTAL; i++) {
		fieldContents.push_back(std::string());
		fieldContents[i] = fpContents[i];
	}
	applyTextToFields();
}

void flightStrip::applyTextToFields() {
	for (int i = 0; i < FIELDS_TOTAL; i++) {
		const unsigned char black[] = { 0,0,0 };
		type.stripTemplate.draw_text(type.fields[i].fieldXLocation, type.fields[i].fieldYLocation, fieldContents[i].c_str(), black, 0, 1, type.fields[i].fieldHeight);
	}
}

#ifdef _DEBUG
void flightStrip::display() {
	type.stripTemplate.save("StripMaker/strip.bmp");
	CImgDisplay main_disp(type.stripTemplate, "Flight Strip",0);
	while (!main_disp.is_closed()) {
		main_disp.wait();
	}
}
#endif