#pragma once
#include <EuroScopePlugIn.h>
#include <string>
#include <vector>
#include "flightStrip.h"

#define MY_PLUGIN_NAME      "StripMaker"
#define MY_PLUGIN_VERSION   "0.1.0"
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

	// handles text commands
	virtual bool OnCompileCommand(const char* sCommandLine);

	// does all the necessary stuff to make and show/print a strip of a given type
	void makeStrip(bool force, bool show, bool print, int type);
	// does all the neccessary stuff to make and show/print a strip with an automatic type
	void makeStrip(bool force, bool show, bool print);

	// gets the required strip type according to type of flight and current strips in use
	int getStripType(EuroScopePlugIn::CFlightPlan flightPlan);

	// gets all the possible fields for a strip, formats them accordingly, and returns them in a string vector.
	std::vector<std::string> getFieldsFromFP();

	// contains list of already printed strips TODO: Delete old disconnected strips
	std::vector<std::string> printedStrips; 

	void openStripSetList();
	void openStripTypeList();

	// does an update every second
	virtual void CStripMakerPlugIn::OnTimer(int Counter);
	
	// returns true if strip has been printed before
	bool stripPrinted(std::string callsign);
	bool showSetList = false;
	bool showTypeList = false;
	RECT menuArea;
};