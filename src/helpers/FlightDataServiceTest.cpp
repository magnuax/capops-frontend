#include "helpers/FlightDataServiceTest.hpp"

FlightDataService::FlightDataService()
{
    // Initialize with dummy data for testing
    _riskStates = {NORMAL, AT_RISK, CONGESTED, NORMAL};
    _weatherStates = {OK, DEGRADED, SEVERE, EXTREME};
    _trafficStates = {LIGHT, MODERATE, HEAVY, LIGHT};
    _sectorIds = {0, 1, 2, 3};
}


RiskState FlightDataService::getRisk(int sectorId) const
{
    return _riskStates[sectorId];
}

WeatherState FlightDataService::getWeather(int sectorId) const
{
    return _weatherStates[sectorId];
}

TrafficState FlightDataService::getTraffic(int sectorId) const
{
    return _trafficStates[sectorId];
}

