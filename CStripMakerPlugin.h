#pragma once
#include <EuroScopePlugIn.h>
#include <string>
#include <vector>
#include "flightStrip.h"

#define MY_PLUGIN_NAME      "StripMaker"
#define MY_PLUGIN_VERSION   "0.1.0-alpha"
#define MY_PLUGIN_DEVELOPER "Samuel Rey"
#define MY_PLUGIN_COPYRIGHT "GPL v3"

class CStripMakerPlugIn :
    public EuroScopePlugIn::CPlugIn
{
public:
    CStripMakerPlugIn();
    //~CStripMakerPlugIn();

    // called by EuroScope when a tag item function is called
    virtual void OnFunctionCall(int FunctionId, const char* sItemString, POINT Pt, RECT Area);

    // called by EuroScope when it needs a tag item value
    virtual void OnGetTagItem(EuroScopePlugIn::CFlightPlan FlightPlan, EuroScopePlugIn::CRadarTarget RadarTarget, int ItemCode, int TagData, char sItemString[16], int* pColorCode, COLORREF* pRGB, double* pFontSize);

    // calls the DisplayUserMessage function to print on the Message channel
    void printMessage(std::string message) {
        DisplayUserMessage("Message", "StripMaker", message.c_str(),TRUE,FALSE,FALSE,FALSE,FALSE);
    }

    // gets the required strip type according to type of flight and current strips in use
    std::vector<stripType>::size_type getStripType();
    // gets all the possible fields for a strip, formats them accordingly, and returns them in a string vector.
    std::vector<std::string> getFieldsFromFP();
};