#pragma once
#include <vector>
#include "domain/SectorStates.hpp"

class IFlightDataService
{
public:
    virtual ~IFlightDataService() = default;
    
    virtual int getSectorId(int row, int col) const = 0;
    virtual std::vector<int> getSectorIds() const = 0;
    
    virtual int getRowCount() const = 0;
    virtual int getColCount() const = 0;
    virtual QSize getGridSize() const = 0;
    virtual QPoint getSectorIndices(int sectorId) const = 0;
    
    virtual RiskState getRisk(int sectorId) const = 0;
    virtual WeatherState getWeather(int sectorId) const = 0;
    virtual TrafficState getTraffic(int sectorId) const = 0;

    virtual std::vector<int> getFlightIds(int sectorId) const = 0;
};
