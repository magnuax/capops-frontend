#pragma once

#include <QString>

class SectorSummary
{
public:
    explicit SectorSummary(
        const int sectorId,
        const int row,
        const int column,
        const int localAircraftCount,
        const int localAircraftBaseCapacity,
        const int localAircraftEffectiveCapacity,
        const QString &weatherSeverity,
        const QString &riskSeverity);

    const int getSectorId() const;
    const int getRow() const;
    const int getColumn() const;
    const int getLocalAircraftCount() const;
    const int getLocalAircraftBaseCapacity() const;
    const int getLocalAircraftEffectiveCapacity() const;
    const QString &getWeatherSeverity() const;
    const QString &getRiskSeverity() const;

private:
    int _sectorId;
    int _row;
    int _column;
    int _localAircraftCount;
    int _localAircraftBaseCapacity;
    int _localAircraftEffectiveCapacity;
    QString _weatherSeverity;
    QString _riskSeverity;
};
