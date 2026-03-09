#pragma once
#include <vector>
#include "domain/SectorStates.hpp"

class IFlightDataService
{
public:
    virtual ~IFlightDataService() = default;

    virtual RiskState getRisk(int sectorId) const = 0;

    virtual WeatherState getWeather(int sectorId) const = 0;

    virtual TrafficState getTraffic(int sectorId) const = 0;

    virtual std::vector<int> getFlightIds(int sectorId) const = 0;
};
