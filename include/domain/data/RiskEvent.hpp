#pragma once

#include <QDateTime>
#include <QString>

#include "domain/SectorStates.hpp"

class RiskEvent
{
public:
    RiskEvent(
        const int riskEventId,
        const int sectorId,
        const bool acknowledged,
        const RiskState &riskState,
        const QDateTime &createdTimestamp,
        const QDateTime &acknowledgedTimestamp,
        const QString &message);

    const int getRiskEventId() const;
    const int getSectorId() const;
    const bool getAcknowledged() const;
    const RiskState &getRiskState() const;
    const QDateTime &getCreatedTimestamp() const;
    const QDateTime &getAcknowledgedTimestamp() const;
    const QString &getMessage() const;

private:
    int _riskEventId;
    int _sectorId;
    bool _acknowledged;
    RiskState _riskState;
    QDateTime _createdTimestamp;
    QDateTime _acknowledgedTimestamp;
    QString _message;
};
