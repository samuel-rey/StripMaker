#pragma once
#include "pch.h"

// function parameters
const int FORCE = 1;
const int SHOW = 1;
const int PRINT = 1;
const int NOFORCE = 0;
const int NOSHOW = 0;
const int NOPRINT = 0;

// stripTypes
const int TYPES_TOTAL = 4;
const int TYPE_DEPARTURE = 0;
const int TYPE_ENROUTE = 1;
const int TYPE_ARRIVAL = 2;
const int TYPE_LOCAL = 3;

// ES tag and function codes
const int TAG_ITEM_PRINT_STATUS = 100;
const int TAG_FUNC_PRINT_STRIP = 101;
const int TAG_FUNC_PRINT_MENU = 102;
const int TAG_FUNC_PRINT_STRIP_FORCE = 103;
const int TAG_FUNC_SHOW_STRIP = 104;
const int TAG_FUNC_STRIP_SET_MENU = 105;
const int TAG_FUNC_CHANGE_STRIP_SET = 106;
const int TAG_FUNC_PRINT_STRIP_AS_TYPE = 107;
const int TAG_FUNC_PRINT_STRIP_TYPE_MENU = 108;
const int TAG_ITEM_SHOW_STRIP_TYPE = 109;

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