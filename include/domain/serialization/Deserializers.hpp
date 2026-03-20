#pragma once

#include <QJsonArray>
#include <QJsonObject>

#include "domain/data/RiskEvent.hpp"
#include "domain/data/MergedRiskEvent.hpp"
#include "domain/data/RiskEventData.hpp"
#include "domain/data/SectorSummary.hpp"
#include "domain/data/SectorSummaryData.hpp"
#include "domain/data/Track.hpp"
#include "domain/data/TrackData.hpp"

namespace Deserializers
{
    RiskEvent toRiskEvent(const QJsonObject &obj);
    MergedRiskEvent toMergedRiskEvent(const QJsonObject &obj, const std::vector<RiskEvent> &events);
    RiskEventData toRiskEventData(const QJsonObject &root);

    SectorSummary toSectorSummary(const QJsonObject &obj);
    SectorSummaryData toSectorSummaryData(const QJsonObject &root);

    Track toTrack(const QJsonObject &obj);
    TrackData toTrackData(const QJsonObject &root);
}