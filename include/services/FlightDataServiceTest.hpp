#pragma once
#include <vector>
#include "services/interfaces/IFlightDataService.hpp"

class QPoint;
class QSize;

class FlightDataServiceTest : public IFlightDataService
{
public:
    FlightDataServiceTest();

    int getSectorId(int row, int col) const override;
    std::vector<int> getSectorIds() const override;

    int getRowCount() const override;
    int getColCount() const override;
    QSize getGridSize() const override;
    QPoint getSectorIndices(int sectorId) const override;

    RiskState getRisk(int sectorId) const override;
    WeatherState getWeather(int sectorId) const override;
    TrafficState getTraffic(int sectorId) const override;

    std::vector<int> getFlightIds(int sectorId) const override;

private:
    std::vector<RiskState> _riskStates;
    std::vector<WeatherState> _weatherStates;
    std::vector<TrafficState> _trafficStates;

    std::vector<int> _sectorIds;
    std::vector<std::vector<int>> _flightIds;
};
