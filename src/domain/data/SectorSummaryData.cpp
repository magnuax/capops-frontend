#include <vector>

#include "domain/data/SectorSummaryData.hpp"
#include "domain/data/SectorSummary.hpp"

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
    _maxLatitude(maxLatitude)
{

    for (const SectorSummary& summary : sectorSummaries)
    {
        _sectorSummaries.insert_or_assign(summary.getSectorId(), summary);
    }

    for (const auto& [sectorId, summary] : _sectorSummaries)
    {
        _sectorIds[QPoint(summary.getRow(), summary.getColumn())] = sectorId;
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

SectorSummary SectorSummaryData::getSectorSummary(int sectorId) const
{
    return _sectorSummaries.at(sectorId);
}

SectorSummary SectorSummaryData::getSectorSummary(int row, int col) const
{
    int sectorId = _sectorIds.at(QPoint(row, col));
    return _sectorSummaries.at(sectorId);
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