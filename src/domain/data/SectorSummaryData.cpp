#include <vector>

#include "domain/data/SectorSummaryData.hpp"
#include "domain/data/SectorSummary.hpp"

SectorSummaryData::SectorSummaryData
(
    const int rowsCount,
    const int columnsCount,
    const double minLatitude,
    const double maxLatitude,
    const double minLongitude,
    const double maxLongitude,
    const std::vector<SectorSummary>& sectorSummaries
)
    :
    _rowsCount(rowsCount),
    _columnsCount(columnsCount),
    _minLongitude(minLongitude),
    _maxLongitude(maxLongitude),
    _minLatitude(minLatitude),
    _maxLatitude(maxLatitude)
{

    for (const SectorSummary& summary : sectorSummaries)
    {
        _sectorSummaries.insert_or_assign(summary.getSectorId(), summary);
    }

    for (const auto& [sectorId, summary] : _sectorSummaries)
    {
        _sectorIds[QPoint(summary.getRow(), summary.getCol())] = sectorId;
    }

}

int SectorSummaryData::getRowCount() const
{
    return _rowsCount;
}

int SectorSummaryData::getColCount() const
{
    return _columnsCount;
}

double SectorSummaryData::getMinLon() const
{
    return _minLongitude;
}

double SectorSummaryData::getMaxLon() const
{
    return _maxLongitude;
}

double SectorSummaryData::getMinLat() const
{
    return _minLatitude;
}

double SectorSummaryData::getMaxLat() const
{
    return _maxLatitude;
}

SectorSummary SectorSummaryData::getSectorSummary(int sectorId) const
{
    auto it = _sectorSummaries.find(sectorId);
    if (it == _sectorSummaries.end())
        return SectorSummary(); // need a default constructor
    return it->second;
}

SectorSummary SectorSummaryData::getSectorSummary(int row, int col) const
{
    auto it = _sectorIds.find(QPoint(row, col));
    if (it == _sectorIds.end())
        return SectorSummary();

    auto it2 = _sectorSummaries.find(it->second);
    if (it2 == _sectorSummaries.end())
        return SectorSummary();

    return it2->second;
}

std::vector<SectorSummary> SectorSummaryData::getSectorSummaries() const
{
    std::vector<SectorSummary> summaries;
 
    for (const auto& [sectorId, summary] : _sectorSummaries)
    {
        summaries.push_back(summary);
    }
 
    return summaries;

}