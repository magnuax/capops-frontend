#include <vector>

#include "domain/data/SectorSummaryData.hpp"
#include "domain/data/SectorSummary.hpp";

SectorSummaryData::SectorSummaryData
(
    const int rowsCount,
    const int columnsCount,
    const double minLongitude,
    const double maxLongitude,
    const double minLatitude,
    const double maxLatitude,
    const std::vector<SectorSummary>& sectorSummaries
)
    :
    _rowsCount(rowsCount),
    _columnsCount(columnsCount),
    _minLongitude(minLongitude),
    _maxLongitude(maxLongitude),
    _minLatitude(minLatitude),
    _maxLatitude(maxLatitude),
    _sectorSummaries(sectorSummaries)
{}

int SectorSummaryData::getRowsCount() const
{
    return _rowsCount;
}

int SectorSummaryData::getColumnsCount() const
{
    return _columnsCount;
}

double SectorSummaryData::getMinLongitude() const
{
    return _minLongitude;
}

double SectorSummaryData::getMaxLongitude() const
{
    return _maxLongitude;
}

double SectorSummaryData::getMinLatitude() const
{
    return _minLatitude;
}

double SectorSummaryData::getMaxLatitude() const
{
    return _maxLatitude;
}

std::vector<SectorSummary> SectorSummaryData::getSectorSummaries() const
{
    return _sectorSummaries;
}