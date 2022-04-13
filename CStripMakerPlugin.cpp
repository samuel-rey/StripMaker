#include "pch.h"
#include "CStripMakerPlugin.h"
#include "logger.h"
#include "CallsignLookup.hpp"
#include "settings.h"
#include "ESHelper.h"
#include "constant.h"
#include <filesystem>
#include <algorithm>

// logger-related variables
bool Logger::ENABLED;

CCallsignLookup* Callsigns = nullptr; // loaded phonetic callsigns

CStripMakerPlugIn::CStripMakerPlugIn(void) :CPlugIn(EuroScopePlugIn::COMPATIBILITY_CODE, MY_PLUGIN_NAME, MY_PLUGIN_VERSION, MY_PLUGIN_DEVELOPER, MY_PLUGIN_COPYRIGHT)
{
	Logger::ENABLED = true;
	Logger::info("Loading StripMaker plugin");

	// set CImg exception mode to 0, this will disable CImg windows with errors as we're handling them ourselves
	cimg_library::cimg::exception_mode(0);

	settings.load();
	
	// register ES tag items & functions
	RegisterTagItemType("Print status", TAG_ITEM_PRINT_STATUS);
	RegisterTagItemType("Strip type", TAG_ITEM_SHOW_STRIP_TYPE);
	RegisterTagItemFunction("Print strip", TAG_FUNC_PRINT_STRIP);
	RegisterTagItemFunction("Print strip as...", TAG_FUNC_PRINT_STRIP_TYPE_MENU);
	RegisterTagItemFunction("Print menu", TAG_FUNC_PRINT_MENU);
	RegisterTagItemFunction("Print Strip (Force)", TAG_FUNC_PRINT_STRIP_FORCE);
	RegisterTagItemFunction("Change strip set", TAG_FUNC_STRIP_SET_MENU);
#ifdef _DEBUG
	RegisterTagItemFunction("(Debug) Show strip", TAG_FUNC_SHOW_STRIP);
#endif
	// load phonetic callsings
	if (Callsigns == nullptr)
		Callsigns = new CCallsignLookup();
	//if (std::filesystem::exists(plugInSettings::customICAOlocation)) { // TODO: try to find phonetic callsigns in the custom location
	//     Callsigns->readFile(plugInSettings::customICAOlocation); 
	//}
	/*else*/ 
	if (std::filesystem::exists("DataFiles\\ICAO_Airlines.txt")) { // try to find phonetic callsigns in ES.exe/DataFiles/ - best option as it will update when controllers .callsign
		Callsigns->readFile("DataFiles\\ICAO_Airlines.txt");
	}
	else if (std::filesystem::exists(settings.dllPath().append("ICAO_Airlines.txt"))) { // else, try to find it in the StripMaker directory
		Callsigns->readFile(settings.dllPath().append("ICAO_Airlines.txt"));
	}
	else if (std::filesystem::exists("ICAO_Airlines.txt")) { // else, try to find it in the ES exe directory
		Callsigns->readFile("ICAO_Airlines.txt");
	}

	Logger::info("Loaded Stripmaker plugin");
}

void CStripMakerPlugIn::OnGetTagItem(EuroScopePlugIn::CFlightPlan FlightPlan, // returns TAG Item values for each TAG Item
	EuroScopePlugIn::CRadarTarget RadarTarget,
	int ItemCode,
	int TagData,
	char sItemString[16],
	int* pColorCode,
	COLORREF* pRGB,
	double* pFontSize) {
	switch (ItemCode) {
	default:
		return;
	case TAG_ITEM_PRINT_STATUS: 
		if (stripPrinted(FlightPlan.GetCallsign())) {
			strcpy_s(sItemString, 16, "PRINTED");
		}
		else {
			strcpy_s(sItemString, 16, "PRINT"); 
		}
		return;
	case TAG_ITEM_SHOW_STRIP_TYPE:
		strcpy_s(sItemString, 16, typeToString(getStripType(FlightPlan)).c_str());
		return;
	}
}

void CStripMakerPlugIn::OnFunctionCall(int FunctionId, // handles TAG Item functions
	const char* sItemString,
	POINT Pt,
	RECT Area) {
	switch (FunctionId) {
	case TAG_FUNC_PRINT_STRIP:
		makeStrip(NOFORCE, NOSHOW,PRINT);
		return;
	case TAG_FUNC_PRINT_MENU:
		OpenPopupList(Area, "Print menu", 1);
		if (stripPrinted(FlightPlanSelectASEL().GetCallsign())) {
			AddPopupListElement("Force print", "", TAG_FUNC_PRINT_STRIP_FORCE);
		}
#ifdef _DEBUG
		AddPopupListElement("(Debug) Show strip", "", TAG_FUNC_SHOW_STRIP);
#endif
		AddPopupListElement("Print as type...", "", TAG_FUNC_PRINT_STRIP_TYPE_MENU);
		AddPopupListElement("Change strip set", "", TAG_FUNC_STRIP_SET_MENU);
		AddPopupListElement(std::string("Type: ").append(typeToString(getStripType(FlightPlanSelectASEL()))).c_str(), "", TAG_FUNC_PRINT_STRIP_TYPE_MENU, false, EuroScopePlugIn::POPUP_ELEMENT_NO_CHECKBOX, true, true);
		return;
	case TAG_FUNC_PRINT_STRIP_FORCE:
		makeStrip(FORCE,NOSHOW,PRINT);
		return;
	case TAG_FUNC_SHOW_STRIP:
		makeStrip(FORCE,SHOW,NOPRINT);
		return;
	case TAG_FUNC_STRIP_SET_MENU:
		// don't even ask.
		menuArea = Area;
		showSetList = true;
		return;
	case TAG_FUNC_CHANGE_STRIP_SET:
		settings.changeCurrentStripSet(sItemString);
		return;
	case TAG_FUNC_PRINT_STRIP_AS_TYPE:
		makeStrip(FORCE, NOSHOW, PRINT, stringToType(sItemString));
		return;
	case TAG_FUNC_PRINT_STRIP_TYPE_MENU:
		menuArea = Area;
		showTypeList = true;
		return;
	}

}

void CStripMakerPlugIn::openStripSetList() {
	// i can't believe I have to do this
	OpenPopupList(menuArea, "Strip sets", 2);
	for each (stripSet set in settings.getStripSets())
	{
		AddPopupListElement(set.setName.c_str(), set.setDescription.c_str(), TAG_FUNC_CHANGE_STRIP_SET, (settings.currentStripSet.setName == set.setName), (settings.currentStripSet.setName == set.setName) ? EuroScopePlugIn::POPUP_ELEMENT_CHECKED : EuroScopePlugIn::POPUP_ELEMENT_UNCHECKED);
	}
	showSetList = false;
	return;
}

void CStripMakerPlugIn::openStripTypeList() {
	// same stuff but for stripTypes
	OpenPopupList(menuArea, "Strip types", 1);
	for (int i = 0; i < TYPES_TOTAL; i++) {
		AddPopupListElement(typeToString(i).c_str(), "", TAG_FUNC_PRINT_STRIP_AS_TYPE, (i == getStripType(FlightPlanSelectASEL())), (i == getStripType(FlightPlanSelectASEL())) ? EuroScopePlugIn::POPUP_ELEMENT_CHECKED : EuroScopePlugIn::POPUP_ELEMENT_UNCHECKED, false, false);
	}
	showTypeList = false;
}

bool CStripMakerPlugIn::OnCompileCommand(const char* sCommandLine) {
	if (strncmp(sCommandLine, ".stripmaker ", strlen(".stripmaker "))) {
		return false;
	}
	// split sCommandLine into a string vector called args
	std::stringstream argStream(sCommandLine);
	std::string arg;
	std::vector<std::string> args;
	while (std::getline(argStream, arg, ' ')) {
		args.push_back(arg);
	}
	if (args.size() == 1) { // if the command is just '.stripmaker, just ignore it
		return true;
	}
	if (args.at(1) == "reloadsettings") {
		Logger::info("Reloading settings");
		settings = plugInSettings::settings();
		settings.load();
	}
	else if (args.at(1) == "printerip" && args.size() == 3) {
		settings.printerIP = args.at(2);
		printMessage(settings.printerIP + " is now the printer IP");
	}
	else if (args.at(1) == "printerport" && args.size() == 3) {
		settings.printerPort = stoi(args.at(2));
		printMessage(std::to_string(settings.printerPort) + " is now the printer port");
	}
	else {
		printMessage("Invalid command");
	}
	return true;
}

void CStripMakerPlugIn::makeStrip(bool force, bool show, bool print, int stripType) {
	if (!stripPrinted(FlightPlanSelectASEL().GetCallsign()) || force) { // if the strip hasn't been printed, or we are in 'force' mode, print the strip and add it to the list.
		try {
			flightStrip strip(stripType, getFieldsFromFP()); // create a strip of the correct type, with the gathered FP info
			if (show) {
				strip.display(); // display the strip if requested
			}
			if (print) {
				strip.print(); // print the strip if required
			}
		}
		catch (std::exception) {
			return;
		}
		printedStrips.push_back(FlightPlanSelectASEL().GetCallsign());
	}
	return;
}

void CStripMakerPlugIn::makeStrip(bool force, bool show, bool print) {
	makeStrip(force, show, print, getStripType(FlightPlanSelectASEL()));
}

int CStripMakerPlugIn::getStripType(EuroScopePlugIn::CFlightPlan flightPlan) { // returns correct strip type, according to the flight type
	if (!strcmp(flightPlan.GetFlightPlanData().GetOrigin(), flightPlan.GetFlightPlanData().GetDestination())) { // if ADEP=ADES, local strip
		return TYPE_LOCAL;
	}
	else if (ControllerMyself().GetFacility() > 1 && ControllerMyself().GetFacility() < 6 || ESHelper::isNearFPAirport(flightPlan) || !ESHelper::isAirborne(flightPlan)) { // if we are DEL/GND/TWR, the ac is close to the airport or in the ground, DEL/ARR strip
		if (ESHelper::depOrArrival(flightPlan)) {
			return TYPE_ARRIVAL;
		}
		else {
			return TYPE_DEPARTURE;
		}
	} 
	else // we are CTR and the aircraft is airborne in cruise, enroute
	{
		return TYPE_ENROUTE;
	}
	return TYPE_DEPARTURE; // default return if no conditions are met
}

std::vector<std::string> CStripMakerPlugIn::getFieldsFromFP() { // gets the fields to fill in the strip from ES and formats them correctly
	std::vector <std::string> obtainedFieldText;
	EuroScopePlugIn::CFlightPlan fp = FlightPlanSelectASEL();
	for (int i = 0; i < FIELDS_TOTAL; i++) {
		obtainedFieldText.push_back(std::string()); // initialize a string to be filled in the switch statement
		switch (i) {
		case FIELD_CALLSIGN: {
			if (fp.GetFlightPlanData().GetCommunicationType() == *"v" || fp.GetFlightPlanData().GetCommunicationType() == *"? ") { // if the communication type is voice or unknown, return the callsign
				obtainedFieldText[i] = fp.GetCallsign();
			}
			else { // otherwise, append the communication type
				std::string commType(1, fp.GetFlightPlanData().GetCommunicationType());
				obtainedFieldText[i] = fp.GetCallsign(); obtainedFieldText[i] += "/"; obtainedFieldText[i] += commType;
			}
			break;
		}
		case FIELD_PHONETIC_CALLSIGN: {
			std::string callsign = fp.GetCallsign();
			obtainedFieldText[i] = Callsigns->getCallsign(callsign.substr(0, 3)); // get callsign from CallsignLookup
			break;
		}
		case FIELD_AIRCRAFT_TYPE: {
			std::string wakeCategory(1, fp.GetFlightPlanData().GetAircraftWtc());
			obtainedFieldText[i] = fp.GetFlightPlanData().GetAircraftFPType(); obtainedFieldText[i] += "/"; obtainedFieldText[i] += wakeCategory;
			break;
		}
		case FIELD_SQUAWK:
			obtainedFieldText[i] = fp.GetControllerAssignedData().GetSquawk();
			break;
		case FIELD_TAS:
			obtainedFieldText[i] = "N" + std::to_string(fp.GetFlightPlanData().GetTrueAirspeed());
			break;
		case FIELD_RFL:
			// convert the altitude in feet to ICAO format
			if (GetTransitionAltitude() > fp.GetFinalAltitude()) { // if we have an altitude, and it's below FL100, the ICAO format is A0 then hundreds of feet
				obtainedFieldText[i] = "A0" + std::to_string(fp.GetFinalAltitude() / 100);
			}
			else if (fp.GetFinalAltitude() / 100 < 100) { // if we have a flight level, and it's below FL100, add a 0 before the hundreds of feet
				obtainedFieldText[i] = "F0" + std::to_string(fp.GetFinalAltitude() / 100);
			}
			else { // otherwise, ICAO format is F then hundreds of feet
				obtainedFieldText[i] = "F"+ std::to_string(fp.GetFinalAltitude() / 100);
			}
			break;
		case FIELD_RULES:
			obtainedFieldText[i] = fp.GetFlightPlanData().GetPlanType();
			break;
		case FIELD_SLOT:
			break;
		case FIELD_EDT:
			obtainedFieldText[i] = fp.GetFlightPlanData().GetEstimatedDepartureTime();
			break;
		case FIELD_ADEP:
			obtainedFieldText[i] = fp.GetFlightPlanData().GetOrigin();
			break;
		case FIELD_ROUTE:
			obtainedFieldText[i] = fp.GetFlightPlanData().GetRoute();
			break;
		case FIELD_ADES:
			obtainedFieldText[i] = fp.GetFlightPlanData().GetDestination();
			break;
		case FIELD_ADEPROUTE: // used for strips that need both combined
			obtainedFieldText[i] = fp.GetFlightPlanData().GetOrigin(); obtainedFieldText[i] += " "; obtainedFieldText[i] += fp.GetFlightPlanData().GetRoute();
			break;
		default:
			obtainedFieldText[i] = "PLACEHOLDER";
		}
	}
	return obtainedFieldText;
}

bool CStripMakerPlugIn::stripPrinted(std::string callsign){
	return !(std::find(printedStrips.begin(), printedStrips.end(), callsign) == printedStrips.end());
}

void CStripMakerPlugIn::OnTimer(int Counter) { 
	// this fires every second and we check... if we have to open a freaking menu. thanks ES.
	// yes. this is dumb.
	if (showSetList) {
		openStripSetList();
	}
	else if (showTypeList)
	{
		openStripTypeList();
	}
}