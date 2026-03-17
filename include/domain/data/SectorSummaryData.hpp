#pragma once

#include <QPoint>
#include "utils/QPointHash.hpp"

#include <vector>
#include <unordered_map>

#include "domain/data/SectorSummary.hpp"

class SectorSummaryData
{

public:
    SectorSummaryData(
        const int rowsCount,
        const int columnsCount,
        const double minLongitude,
        const double maxLongitude,
        const double minLatitude,
        const double maxLatitude,
        const std::vector<SectorSummary> &sectorSummaries);

    int getRowCount() const;
    int getColCount() const;
    
    double getMinLongitude() const;
    double getMaxLongitude() const;
    double getMinLatitude() const;
    double getMaxLatitude() const;

    SectorSummary getSectorSummary(int sectorId) const;
    SectorSummary getSectorSummary(int row, int col) const;
    std::vector<SectorSummary> getSectorSummaries() const;

private:
    int _rowsCount;
    int _columnsCount;

    double _minLongitude;
    double _maxLongitude;
    double _minLatitude;
    double _maxLatitude;

    std::unordered_map<QPoint, int, QPointHash> _sectorIds;
    std::unordered_map<int, SectorSummary> _sectorSummaries;
};