#pragma once

#include <vector>

#include "domain/data/SectorSummary.hpp"

class SectorSummaryData
{
public:
    SectorSummaryData
    (
        const int rowsCount,
        const int columnsCount,
        const double minLongitude,
        const double maxLongitude,
        const double minLatitude,
        const double maxLatitude,
        const std::vector<SectorSummary>& sectorSummaries
    );

    int getRowsCount() const;
    int getColumnsCount() const;
    double getMinLongitude() const;
    double getMaxLongitude() const;
    double getMinLatitude() const;
    double getMaxLatitude() const;
    std::vector<SectorSummary> getSectorSummaries() const;

private:
    int _rowsCount;
    int _columnsCount;
    double _minLongitude;
    double _maxLongitude;
    double _minLatitude;
    double _maxLatitude;
    std::vector<SectorSummary> _sectorSummaries;
};