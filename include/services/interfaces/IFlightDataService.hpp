#pragma once
#include <QObject>
#include <vector>

#include "domain/data/RiskEventData.hpp"
#include "domain/data/SectorSummaryData.hpp"
#include "domain/data/TrackData.hpp"

class IFlightDataService : public QObject
{
    Q_OBJECT

signals:
    void dataReloaded();
    void acknowledgeSucceeded(const MergedRiskEvent &mergedEvent);
    void acknowledgeFailed(const MergedRiskEvent &mergedEvent);

public:
    using QObject::QObject;
    virtual ~IFlightDataService() = default;

    virtual SectorSummaryData getSectorSummaryData() const = 0;
    virtual TrackData getTrackData() const = 0;
    virtual RiskEventData getRiskEventData() const = 0;

    virtual void acknowledgeRiskEvents(const MergedRiskEvent &mergedEvent) = 0;
};