#include <unordered_map>
#include <vector>

#include "domain/data/RiskEventData.hpp"
#include "domain/data/RiskEvent.hpp"
#include "domain/data/MergedRiskEvent.hpp"

RiskEventData::RiskEventData
(
    const int riskEventCount,
    const std::vector<RiskEvent>& riskEvents
)
    : _riskEventCount(riskEventCount)
{
    mergeRiskEvents();
}

int RiskEventData::getRiskEventCount() const
{
    return _riskEventCount;
}

std::vector<RiskEvent> RiskEventData::getRiskEvents() const
{
    return _riskEvents;
}

std::vector<MergedRiskEvent> RiskEventData::getMergedRiskEvents() const
{
    return _mergedRiskEvents;
}

void RiskEventData::mergeRiskEvents()
{
    std::unordered_map<int, std::vector<RiskEvent>> riskEventsPerSector;

    for (RiskEvent riskEvent : _riskEvents)
    {
        int sectorId = riskEvent.getSectorId();
        riskEventsPerSector[sectorId].push_back(riskEvent);
    }

    _mergedRiskEvents.clear();
    _mergedRiskEvents.reserve(riskEventsPerSector.size());
    for (const auto& [sectorId, riskEvents] : riskEventsPerSector)
    {
        _mergedRiskEvents.push_back(MergedRiskEvent(sectorId, riskEvents));
    }
}