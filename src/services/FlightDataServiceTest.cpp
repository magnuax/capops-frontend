#include <QPoint>
#include <QSize>

#include "services/FlightDataServiceTest.hpp"

FlightDataServiceTest::FlightDataServiceTest()
{
    // Initialize with dummy data for testing
    _riskStates = {NORMAL, AT_RISK, CONGESTED, NORMAL};
    _weatherStates = {OK, DEGRADED, SEVERE, EXTREME};
    _trafficStates = {LIGHT, MODERATE, HEAVY, LIGHT};
    _sectorIds = {0, 1, 2, 3};
    _flightIds = {
        {101, 102, 103},
        {201, 202},
        {301, 302, 303, 304},
        {401}
    };
}

int FlightDataServiceTest::getRowCount() const
{
    return 2;
}

int FlightDataServiceTest::getColCount() const
{
    return 2;
}

QSize FlightDataServiceTest::getGridSize() const
{
    return QSize(getColCount(), getRowCount());
}

RiskState FlightDataServiceTest::getRisk(int sectorId) const
{
    return _riskStates[sectorId];
}

WeatherState FlightDataServiceTest::getWeather(int sectorId) const
{
    return _weatherStates[sectorId];
}

TrafficState FlightDataServiceTest::getTraffic(int sectorId) const
{
    return _trafficStates[sectorId];
}

QPoint FlightDataServiceTest::getSectorIndices(int sectorId) const
{
    const int row = sectorId / getColCount();
    const int col = sectorId % getColCount();
    return {row, col};
}

int FlightDataServiceTest::getSectorId(int row, int col) const
{
    return _sectorIds[row * getColCount() + col];
}

std::vector<int> FlightDataServiceTest::getSectorIds() const
{
    return _sectorIds;
}

std::vector<int> FlightDataServiceTest::getFlightIds(int sectorId) const
{
    return _flightIds[sectorId];
}