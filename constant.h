#pragma once
#include "pch.h"

// function parameters
#define FORCE 1
#define SHOW 1
#define PRINT 1
#define NOFORCE 0
#define NOSHOW 0
#define NOPRINT 0

// stripTypes
const int TYPES_TOTAL = 4;
const int TYPE_DEPARTURE = 0;
const int TYPE_ENROUTE = 1;
const int TYPE_ARRIVAL = 2;
const int TYPE_OVERFLIGHT = 3;

// ES tag and functions codes
const int TAG_ITEM_PRINT_STATUS = 100;
const int TAG_FUNC_PRINT_STRIP = 101;
const int TAG_FUNC_PRINT_MENU = 102;
const int TAG_FUNC_PRINT_STRIP_FORCE = 103;
const int TAG_FUNC_SHOW_STRIP = 104;

// field types codes
const int FIELDS_TOTAL = 13;
const int FIELD_CALLSIGN = 0;
const int FIELD_PHONETIC_CALLSIGN = 1;
const int FIELD_AIRCRAFT_TYPE = 2;
const int FIELD_SQUAWK = 3;
const int FIELD_TAS = 4;
const int FIELD_RFL = 5;
const int FIELD_RULES = 6;
const int FIELD_SLOT = 7;
const int FIELD_EDT = 8;
const int FIELD_ADEP = 9;
const int FIELD_ROUTE = 10;
const int FIELD_ADES = 11;
const int FIELD_ADEPROUTE = 12;