#include <QString>

#include "domain/data/SectorSummary.hpp"

SectorSummary::SectorSummary(
    const int sectorId,
    const int row,
    const int column,
    const std::vector<QString> aircraftIds,
    const TrafficState &trafficState,
    const WeatherState &weatherState,
    const RiskState &riskState)
    : _sectorId(sectorId),
      _row(row),
      _column(column),
      _aircraftIds(aircraftIds),
      _trafficState(trafficState),
      _weatherState(weatherState),
      _riskState(riskState)
{
    _localAircraftCount = aircraftIds.size();
}

const int SectorSummary::getSectorId() const
{
    return _sectorId;
}

const int SectorSummary::getRow() const
{
    return _row;
}

const int SectorSummary::getColumn() const
{
    return _column;
}

const std::vector<QString> &SectorSummary::getAircraftIds() const
{
    return _aircraftIds;
}

const int SectorSummary::getAircraftCount() const
{
    return _localAircraftCount;
}

const TrafficState &SectorSummary::getTrafficState() const
{
    return _trafficState;
}

const WeatherState &SectorSummary::getWeatherState() const
{
    return _weatherState;
}
const RiskState &SectorSummary::getRiskState() const
{
    return _riskState;
}
