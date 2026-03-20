
#include <string>
#include <vector>

#include <QString>

#include "domain/data/MergedRiskEvent.hpp"
#include "domain/data/RiskEvent.hpp"

MergedRiskEvent::MergedRiskEvent() : _sectorId(-1), _riskEvents({})
{
}

MergedRiskEvent::MergedRiskEvent(
    const int sectorId,
    const std::vector<RiskEvent> &riskEvents)
    : _sectorId(sectorId), _riskEvents(riskEvents)
{
    updateSummaryInfo();
}

int MergedRiskEvent::getSectorId() const
{
    return _sectorId;
}

std::vector<RiskEvent> MergedRiskEvent::getRiskEvents() const
{
    return _riskEvents;
}

QString MergedRiskEvent::getSummaryMessage() const
{
    return _summaryMessage;
}

QString MergedRiskEvent::getLastMessage() const
{
    return _lastMessage;
}

const RiskEvent &MergedRiskEvent::getLastEvent() const
{
    return *std::max_element(
        _riskEvents.begin(), _riskEvents.end(),
        [](const RiskEvent &a, const RiskEvent &b)
        {
            return a.getCreatedTimestamp() < b.getCreatedTimestamp();
        });
}

RiskState MergedRiskEvent::getLastRiskState() const
{
    return getLastEvent().getRiskState();
}

void MergedRiskEvent::setRiskEvents(const std::vector<RiskEvent> &riskEvents)
{
    _riskEvents = riskEvents;
    updateSummaryInfo();
}

void MergedRiskEvent::addRiskEvents(const std::vector<RiskEvent> &riskEvents)
{
    for (RiskEvent riskEvent : riskEvents)
    {
        addRiskEvent(riskEvent);
    }

    updateSummaryInfo();
}

void MergedRiskEvent::addRiskEvent(const RiskEvent &riskEvent)
{
    _riskEvents.push_back(riskEvent);
}

void MergedRiskEvent::setSummaryInfo(
    const RiskEvent *lastRiskEvent,
    const RiskEvent *secondLastRiskEvent)
{
    // for readability
    bool lastExists = !!lastRiskEvent;
    bool secondLastExists = !!secondLastRiskEvent;

    if (!lastExists && !secondLastExists)
    {
        _currentState = RiskState::UNKNOWN;
        _previousState = RiskState::UNKNOWN;
        _summaryMessage = QString("Risk severity in sector %1 unknown").arg(_sectorId);
    }

    else if (lastExists && !secondLastExists)
    {
        _currentState = lastRiskEvent->getRiskState();
        _lastMessage = lastRiskEvent->getMessage();
        _summaryMessage = QString("Risk severity in sector %1 changed to %2").arg(_sectorId).arg(toString(_currentState));
    }

    else if (lastExists && secondLastExists)
    {
        _currentState = lastRiskEvent->getRiskState();
        _previousState = secondLastRiskEvent->getRiskState();
        _summaryMessage = QString("Risk severity in sector %1 changed from %2 to %3")
                              .arg(_sectorId)
                              .arg(toString(_previousState))
                              .arg(toString(_currentState));
    }

    else
    {
        _currentState = RiskState::UNKNOWN;
        _previousState = RiskState::UNKNOWN;
        throw std::logic_error(
            "Unexpected behavior: secondLastRiskEvent exists without lastRiskEvent "
            "in MergedRiskEvent::setSummaryInfo");
    }
}

void MergedRiskEvent::updateSummaryInfo()
{
    RiskEvent *lastRiskEvent = nullptr;
    RiskEvent *secondLastRiskEvent = nullptr;

    for (RiskEvent riskEvent : _riskEvents)
    {
        if (!lastRiskEvent)
        {
            lastRiskEvent = &riskEvent;
            continue;
        }

        if (riskEvent.getCreatedTimestamp() > lastRiskEvent->getCreatedTimestamp())
        {
            secondLastRiskEvent = lastRiskEvent;
            lastRiskEvent = &riskEvent;
        }
    }

    setSummaryInfo(lastRiskEvent, secondLastRiskEvent);
}