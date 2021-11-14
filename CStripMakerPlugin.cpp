#include "pch.h"
#include "CStripMakerPlugin.h"
#include "logger.h"
#include "CallsignLookup.hpp"
#include "settings.h"
#include "constant.h"
#include <filesystem>
#include <algorithm>

// logger-related variables
bool Logger::ENABLED;
std::string Logger::DLL_PATH;

CCallsignLookup* Callsigns = nullptr; // loaded phonetic callsigns
std::vector<std::string> printedStrips; // contains list of already printed strips TODO: Delete old disconnected strips

CStripMakerPlugIn::CStripMakerPlugIn(void) :CPlugIn(EuroScopePlugIn::COMPATIBILITY_CODE, MY_PLUGIN_NAME, MY_PLUGIN_VERSION, MY_PLUGIN_DEVELOPER, MY_PLUGIN_COPYRIGHT)
{
    // set CImg exception mode to 0, this will disable CImg windows with errors as we're handling them ourselves
    cimg_library::cimg::exception_mode(0);

    // load plugin settings
    
    settings.load();
    
	// register ES tag items & functions
	RegisterTagItemType("Print status", TAG_ITEM_PRINT_STATUS);
	RegisterTagItemFunction("Print strip", TAG_FUNC_PRINT_STRIP);
    RegisterTagItemFunction("Print menu", TAG_FUNC_PRINT_MENU);
    RegisterTagItemFunction("Print Strip (Force)", TAG_FUNC_PRINT_STRIP_FORCE);
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

    // set up logger
    Logger::DLL_PATH = settings.dllPath();
    Logger::ENABLED = TRUE;
    Logger::info("Loaded Stripmaker plugin successfully");
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
        if (!(std::find(printedStrips.begin(), printedStrips.end(), FlightPlan.GetCallsign()) == printedStrips.end())) { // if the strip has already already been printed, return "PRINTED"
            strcpy_s(sItemString, 16, "PRINTED");
        }
        else {// otherwise, return "PRINT"
            strcpy_s(sItemString, 16, "PRINT"); 
        }
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
        AddPopupListElement("Force print", "", TAG_FUNC_PRINT_STRIP_FORCE);
#ifdef _DEBUG
        AddPopupListElement("(Debug) Show strip", "", TAG_FUNC_SHOW_STRIP);
#endif
        return;
    case TAG_FUNC_PRINT_STRIP_FORCE:
        makeStrip(FORCE,NOSHOW,PRINT);
        return;
    case TAG_FUNC_SHOW_STRIP:
        makeStrip(FORCE,SHOW,NOPRINT);
        return;
    }
}

void CStripMakerPlugIn::makeStrip(bool force, bool show, bool print) {
    if ((std::find(printedStrips.begin(), printedStrips.end(), FlightPlanSelectASEL().GetCallsign()) == printedStrips.end()) || force) { // if the strip hasn't been printed, or we are in 'force' mode, print the strip and add it to the list.
        printedStrips.push_back(FlightPlanSelectASEL().GetCallsign());
        flightStrip strip(getStripType(), getFieldsFromFP()); // create a strip of the correct type, with the gathered FP info
        if (show) {
            strip.display(); // display the strip if requested
        }
        if (print) {
            //strip.print(); // print the strip if required
        }
    }
    return;
}

std::vector<stripType>::size_type CStripMakerPlugIn::getStripType() { // returns correct strip type, according to the flight type
    return TYPE_DEPARTURE; // for now, we only have one, hardwired, strip type. so that's the correct strip type to use.
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
            if (GetTransitionAltitude() > fp.GetFinalAltitude()) { // if we have an altitude, and it's below TL, the ICAO format is A0 then hundreds of feet
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