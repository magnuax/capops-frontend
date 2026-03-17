#pragma once

#include <vector>

#include <QString>

class RiskEvent;

class MergedRiskEvent
{
public:
    MergedRiskEvent(
        const int sectorId,
        const std::vector<RiskEvent> &riskEvents);

    int getSectorId() const;
    std::vector<RiskEvent> getRiskEvents() const;
    QString getSummaryMessage() const;
    QString getLastMessage() const;

    void setRiskEvents(const std::vector<RiskEvent> &riskEvents);
    void addRiskEvents(const std::vector<RiskEvent> &riskEvents);

private:
    int _sectorId;
    std::vector<RiskEvent> _riskEvents;
    QString _summaryMessage;
    QString _lastMessage;
    QString _previousSeverity;
    QString _currentSeverity;

    void addRiskEvent(const RiskEvent &riskEvent);
    void updateSummaryInfo();
    void setSummaryInfo(
        const RiskEvent *lastRiskEvent = nullptr,
        const RiskEvent *secondLastRiskEvent = nullptr);
};