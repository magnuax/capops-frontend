#pragma once

#include <QString>

#include "domain/SectorStates.hpp"

class SectorSummary
{
public:
    SectorSummary() = default;

    explicit SectorSummary(
        const int sectorId,
        const int row,
        const int column,
        const std::vector<QString> aircraftIds,
        const TrafficState &TrafficState,
        const WeatherState &weatherState,
        const RiskState &riskState);

    const int getSectorId() const;
    const int getRow() const;
    const int getCol() const;

    const std::vector<QString> &getAircraftIds() const;
    const int getAircraftCount() const;

    const TrafficState &getTrafficState() const;
    const WeatherState &getWeatherState() const;
    const RiskState &getRiskState() const;

private:
    int _sectorId;
    int _row;
    int _column;
    int _localAircraftCount;

    std::vector<QString> _aircraftIds;

    TrafficState _trafficState;
    WeatherState _weatherState;
    RiskState _riskState;
};
