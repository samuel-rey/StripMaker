#pragma once
#include <EuroScopePlugIn.h>
#include <string>

#define MY_PLUGIN_NAME      "StripMaker"
#define MY_PLUGIN_VERSION   "Alpha 0.1.0"
#define MY_PLUGIN_DEVELOPER "Samuel Rey"
#define MY_PLUGIN_COPYRIGHT "GPL v3"

class CStripMakerPlugIn :
    public EuroScopePlugIn::CPlugIn
{
public:
    CStripMakerPlugIn();
    //~CStripMakerPlugIn();

    //---OnFunctionCall------------------------------------------
    //virtual void OnFunctionCall(int FunctionId, const char* sItemString, POINT Pt, RECT Area);

    //---OnGetTagItem------------------------------------------
    //virtual void OnGetTagItem(EuroScopePlugIn::CFlightPlan FlightPlan, EuroScopePlugIn::CRadarTarget RadarTarget, int ItemCode, int TagData, char sItemString[16], int* pColorCode, COLORREF* pRGB, double* pFontSize);

    //Calls the DisplayUserMessage function to print on the Message channel
    void printMessage(std::string message) {
        DisplayUserMessage("Message", "StripMaker", message.c_str(),TRUE,FALSE,FALSE,FALSE,FALSE);
    }
};