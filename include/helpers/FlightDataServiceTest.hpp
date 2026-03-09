#pragma once
#include <vector>
#include "helpers/IFlightDataService.hpp"

class FlightDataService : public IFlightDataService
{
public:
    FlightDataService();

    RiskState getRisk(int sectorId) const override;

    WeatherState getWeather(int sectorId) const override;

    TrafficState getTraffic(int sectorId) const override;

    std::vector<int> getFlightIds(int sectorId) const override;

private:
    std::vector<RiskState> _riskStates;
    std::vector<WeatherState> _weatherStates;
    std::vector<TrafficState> _trafficStates;

    std::vector<int> _sectorIds;
};
