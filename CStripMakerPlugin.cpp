#include "pch.h"
#include "CStripMakerPlugin.h"
#include <spdlog/spdlog.h>
#include <spdlog/sinks/basic_file_sink.h>
#include "CallsignLookup.hpp"
#include "loadSettings.h"
#include "constant.h"
#include <filesystem>

std::shared_ptr<spdlog::logger> logger; // local instance of logger
CCallsignLookup* Callsigns = nullptr; // loaded phonetic callsigns
std::vector<std::string> printedStrips;

CStripMakerPlugIn::CStripMakerPlugIn(void) :CPlugIn(EuroScopePlugIn::COMPATIBILITY_CODE, MY_PLUGIN_NAME, MY_PLUGIN_VERSION, MY_PLUGIN_DEVELOPER, MY_PLUGIN_COPYRIGHT)
{
	// set up logger
	auto logger = spdlog::basic_logger_mt("StripMaker", "StripMaker/logs/log.txt");
	spdlog::flush_every(std::chrono::seconds(5));
	spdlog::flush_on(spdlog::level::err);

	// load plugin settings
	logger->info("StripMaker plugin starting up...");
	plugInSettings::loadSettings();
	logger->info("Succesfully loaded {} strip types", plugInSettings::getTypes().size());

	// register ES tag items & functions
	RegisterTagItemType("Print status", TAG_ITEM_PRINT_STATUS);
	RegisterTagItemFunction("Print strip", TAG_FUNC_PRINT_STRIP);

	// load phonetic callsings
	if (Callsigns == nullptr)
		Callsigns = new CCallsignLookup();
	if (std::filesystem::exists("ICAO_Airlines.txt")) {
		Callsigns->readFile("ICAO_Airlines.txt");
	}
}

void CStripMakerPlugIn::OnGetTagItem(EuroScopePlugIn::CFlightPlan FlightPlan,
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
        if (!(std::find(printedStrips.begin(), printedStrips.end(), FlightPlan.GetCallsign()) == printedStrips.end())) {
            strcpy(sItemString, "PRINTED");
        }
        else {
            strcpy(sItemString, "PRINT");
        }
        return;
    }
}

void CStripMakerPlugIn::OnFunctionCall(int FunctionId,
    const char* sItemString,
    POINT Pt,
    RECT Area) {
    switch (FunctionId) {
    case TAG_FUNC_PRINT_STRIP:
        flightStrip strip(plugInSettings::getTypes()[getStripType()], getFieldsFromFP());
        printedStrips.push_back(FlightPlanSelectASEL().GetCallsign());
        return;
    }
}

std::vector<stripType>::size_type CStripMakerPlugIn::getStripType() {
    return 0;
}
}