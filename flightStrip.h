#pragma once
#include <CImg.h>
#include <string>
#include <vector>
#include <EuroScopePlugIn.h>

using namespace cimg_library;

const int FIELD_CALLSIGN = 0;
const int FIELD_PHONETIC_CALLSIGN = 1;
const int FIELD_AIRCRAFT_TYPE = 2;
const int FIELD_SQWAWK = 3;
const int FIELD_TAS = 4;
const int FIELD_RFL = 5;
const int FIELD_RULES = 6;
const int FIELD_SLOT = 7;
const int FIELD_EDT = 8;
const int FIELD_ADEP = 9;
const int FIELD_ROUTE = 10;
const int FIELD_ADES = 11;
const int FIELD_ADEPROUTE = 12;

// a stripField is an individual field that will be filled out with info from the aircraft.
struct stripField {
	int fieldXLocation;
	int fieldYLocation;
	int fieldHeight;
};

// a stripType is the settings for a particular strip. We can set which image is used as a template and where each field goes in that image.
struct stripType {
	std::string stripName;
	CImg<unsigned int> stripTemplate;
	std::vector<stripField> fields;
};

// this contains all the flight data in the correct format and the strip data, that is, the template for the particular strip type. It also contains the functions for making the strip image.
class flightStrip
{
public:
	stripType type;
	std::vector<std::string> fieldContents;
	flightStrip(stripType type, std::vector<std::string> fpContents); // constructor for this class. Creates instance of flightStrip with type 'type' and populates fieldContents with the info from 'fpContents'
	void applyTextToFields(); // takes the stripTemplate and applies the fieldContents to it according to the settings from its stripType
};

