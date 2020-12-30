#include "pch.h"
#include "CStripMakerPlugin.h"
#include "logger.h"
#include "CallsignLookup.hpp"
#include "loadSettings.h"
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
    // load plugin settings
    plugInSettings::loadSettings();

	// register ES tag items & functions
	RegisterTagItemType("Print status", TAG_ITEM_PRINT_STATUS);
	RegisterTagItemFunction("Print strip", TAG_FUNC_PRINT_STRIP);

	// load phonetic callsings
	if (Callsigns == nullptr)
		Callsigns = new CCallsignLookup();
	if (std::filesystem::exists("ICAO_Airlines.txt")) {
		Callsigns->readFile("ICAO_Airlines.txt");
	}
    char DllPathFile[_MAX_PATH];
    std::string DllPath;
    GetModuleFileNameA(HINSTANCE(&__ImageBase), DllPathFile, sizeof(DllPathFile));
    DllPath = DllPathFile;
    DllPath.resize(DllPath.size() - strlen("StripMaker.dll"));
    Logger::DLL_PATH = DllPath;
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
#ifndef _DEBUG 
        if (!(std::find(printedStrips.begin(), printedStrips.end(), FlightPlanSelectASEL().GetCallsign()) == printedStrips.end())) { // if the strip has already been printed, don't execute the function
            return;
        }
#endif // _DEBUG
        flightStrip strip(plugInSettings::getTypes()[getStripType()], getFieldsFromFP()); // create a strip of the correct type, with the gathered FP info
//#ifdef _DEBUG
        strip.display(); // display the strip in a window
//#endif
        printedStrips.push_back(FlightPlanSelectASEL().GetCallsign()); // add the aircraft to the list of printed strips
        return;
    }
}

std::vector<stripType>::size_type CStripMakerPlugIn::getStripType() { // returns correct strip type, according to the flight type
    return 0; // for now, we only have one, hardwired, strip type. so that's the correct strip type to use.
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
        case FIELD_SQWAWK:
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