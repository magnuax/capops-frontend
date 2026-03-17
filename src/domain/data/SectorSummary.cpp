#include <QString>

#include "domain/data/SectorSummary.hpp"

SectorSummary::SectorSummary
(
    const int sectorId,
    const int row,
    const int colunmn,
    const int localAircraftCount,
    const int localAircraftBaseCapacity,
    const int localAircraftEffectiveCapacity,
    const QString& weatherSeverity,
    const QString& riskSeverity
)   
    :
    _sectorId(sectorId),
    _row(row),
    _colunmn(colunmn),
    _localAircraftCount(localAircraftCount),
    _localAircraftBaseCapacity(localAircraftBaseCapacity),
    _localAircraftEffectiveCapacity(localAircraftEffectiveCapacity),
    _weatherSeverity(weatherSeverity),
    _riskSeverity(riskSeverity)
{}

const int SectorSummary::getSectorId() const
{
    return _sectorId;
}

const int SectorSummary::getRow() const
{
    return _row;
}

const int SectorSummary::getColunmn() const
{
    return _colunmn;
}

const int SectorSummary::getLocalAircraftCount() const
{
    return _localAircraftCount;
}

const int SectorSummary::getLocalAircraftBaseCapacity() const
{
    return _localAircraftBaseCapacity;
}

const int SectorSummary::getLocalAircraftEffectiveCapacity() const
{
    return _localAircraftEffectiveCapacity;
}

const QString& SectorSummary::getWeatherSeverity() const
{
    return _weatherSeverity;
}
const QString& SectorSummary::getRiskSeverity() const
{
    return _riskSeverity;
}
