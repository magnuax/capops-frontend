#pragma once

#include <vector>

#include <QString>
#include "domain/SectorStates.hpp"

class RiskEvent;

class MergedRiskEvent
{
public:
    MergedRiskEvent();

    MergedRiskEvent(
        const int sectorId,
        const std::vector<RiskEvent> &riskEvents);

    int getSectorId() const;
    std::vector<RiskEvent> getRiskEvents() const;
    QString getSummaryMessage() const;
    
    QString getLastMessage() const;
    const RiskEvent &getLastEvent() const;
    RiskState getLastRiskState() const;

    void setRiskEvents(const std::vector<RiskEvent> &riskEvents);
    void addRiskEvents(const std::vector<RiskEvent> &riskEvents);

private:
    int _sectorId;
    std::vector<RiskEvent> _riskEvents;
    QString _summaryMessage;
    QString _lastMessage;
    RiskState _previousState;
    RiskState _currentState;

    void addRiskEvent(const RiskEvent &riskEvent);
    void updateSummaryInfo();
    void setSummaryInfo(
        const RiskEvent *lastRiskEvent = nullptr,
        const RiskEvent *secondLastRiskEvent = nullptr);
};