#pragma once
#include "constant.h"
#pragma warning(push, 0)
#include <CImg.h>
#pragma warning(pop)
#include <string>
#include <vector>
#include <EuroScopePlugIn.h>

using namespace cimg_library;

// a stripField is an individual field that will be filled out with info from the aircraft.
struct stripField {
	int fieldXLocation;
	int fieldYLocation;
	int fieldHeight;
};

// a stripType is the settings for a particular strip of a particular situation, such as departure. We can set which image is used as a template and where each field goes in that image.
struct stripType {
	std::string templateFile;
	stripField fields[FIELDS_TOTAL];
};

// convert from type-index to type-name
std::string typeToString(int type);
int stringToType(std::string typeName);

// a stripSet is a set of types that all share a common theme. The user uses one stripSet when controlling w/ the plugin
struct stripSet {
	std::string setName;
	std::string setDescription;
	stripType type[TYPES_TOTAL];
};

// this contains all the flight data in the correct format and the strip data, that is, the template for the particular strip type. It also contains the functions for making the strip image.
class flightStrip
{
private:
	void applyTextToFields(); // takes the stripTemplate and applies the fieldContents to it according to the settings from its stripType
public:
	CImg<unsigned int> stripTemplate;
	stripField fields[FIELDS_TOTAL];
	std::vector<std::string> fieldContents;
	flightStrip(int type, std::vector<std::string> fpContents); // constructor for this class. Creates instance of flightStrip with type 'type' and populates fieldContents with the info from 'fpContents'
	void print(); // prints strip out to paper
	void display(); // displays strip in window
};

