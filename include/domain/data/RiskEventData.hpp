#pragma once

#include <vector>

#include "domain/data/RiskEvent.hpp"
#include "domain/data/MergedRiskEvent.hpp"

class RiskEventData
{
public:
    RiskEventData();

    RiskEventData(
        const int riskEventCount,
        const std::vector<RiskEvent> &riskEvents,
        const std::vector<MergedRiskEvent> &mergedRiskEvents = {});

    int getRiskEventCount() const;
    std::vector<RiskEvent> getRiskEvents() const;
    std::vector<MergedRiskEvent> getMergedRiskEvents() const;

private:
    int _riskEventCount;
    std::vector<RiskEvent> _riskEvents;
    std::vector<MergedRiskEvent> _mergedRiskEvents;

    void mergeRiskEvents();
};
