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
        const double minLatitude,
        const double maxLatitude,
        const double maxLongitude,
        const double minLongitude,
        const std::vector<SectorSummary> &sectorSummaries);

    int getRowCount() const;
    int getColCount() const;
    
    double getMinLon() const;
    double getMaxLon() const;
    double getMinLat() const;
    double getMaxLat() const;

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