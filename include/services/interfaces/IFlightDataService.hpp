#pragma once
#include <vector>

#include "domain/SectorStates.hpp"
#include "domain/data/SectorSummaryData.hpp"
#include "domain/data/RiskEventData.hpp"
#include "domain/data/TrackData.hpp"

class IFlightDataService
{
public:
    virtual ~IFlightDataService() = default;

    virtual SectorSummaryData getSectorSummaryData() const = 0;
    virtual RiskEventData getRiskEventData() const = 0;
    virtual TrackData getTrackData() const = 0;
};
