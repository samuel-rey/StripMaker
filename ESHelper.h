#pragma once
#include <EuroScopePlugIn.h>
#include <string>

namespace ESHelper {
	// returns true if aircraft is airborne, i.e. over 500ft MSL and GS>50
	bool isAirborne(EuroScopePlugIn::CFlightPlan flightPlan);

	// returns false for departure and true for arrival, checks by comparing distance from origin and destination
	bool depOrArrival(EuroScopePlugIn::CFlightPlan flightPlan);

	// returns true if plane is within 25nm of origin/destination airport
	bool isNearFPAirport(EuroScopePlugIn::CFlightPlan flightPlan);
}