#include "pch.h"
#include "flightStrip.h"
#include "constant.h"#ifdef _DEBUG
void flightStrip::display() {
	type.stripTemplate.save("StripMaker/strip.bmp");
	CImgDisplay main_disp(type.stripTemplate, "Flight Strip",0);
	while (!main_disp.is_closed()) {
		main_disp.wait();
	}
}
#endif