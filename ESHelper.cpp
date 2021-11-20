#include "pch.h"
#include "ESHelper.h"
#include "settings.h"
#include "flightStrip.h"
#include <EuroScopePlugIn.h>
#include <string>

namespace ESHelper {
	bool isAirborne(EuroScopePlugIn::CFlightPlan flightPlan) {
		if (flightPlan.GetFPTrackPosition().GetPressureAltitude() > 500 && flightPlan.GetFPTrackPosition().GetReportedGS() > 50) {
			return true;
		}
		else {
			return false;
		}
	}

	bool depOrArrival(EuroScopePlugIn::CFlightPlan flightPlan) {
		return (flightPlan.GetDistanceFromOrigin() > flightPlan.GetDistanceToDestination());
	}

	bool isNearFPAirport(EuroScopePlugIn::CFlightPlan flightPlan) {
		int test = flightPlan.GetDistanceFromOrigin();
		int test2 = flightPlan.GetDistanceToDestination();
		if (flightPlan.GetDistanceFromOrigin() < 25 || flightPlan.GetDistanceToDestination() < 25) {
			return true;
		}
		return false;
	}
}