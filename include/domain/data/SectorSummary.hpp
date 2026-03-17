#pragma once

#include <QString>

class SectorSummary
{
public:
    explicit SectorSummary(
        const int sectorId,
        const int row,
        const int colunmn,
        const int localAircraftCount,
        const int localAircraftBaseCapacity,
        const int localAircraftEffectiveCapacity,
        const QString &weatherSeverity,
        const QString &riskSeverity);

    const int getSectorId() const;
    const int getRow() const;
    const int getColunmn() const;
    const int getLocalAircraftCount() const;
    const int getLocalAircraftBaseCapacity() const;
    const int getLocalAircraftEffectiveCapacity() const;
    const QString &getWeatherSeverity() const;
    const QString &getRiskSeverity() const;

private:
    int _sectorId;
    int _row;
    int _colunmn;
    int _localAircraftCount;
    int _localAircraftBaseCapacity;
    int _localAircraftEffectiveCapacity;
    QString _weatherSeverity;
    QString _riskSeverity;
};
