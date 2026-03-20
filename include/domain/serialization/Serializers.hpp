#pragma once

#include <QJsonObject>
#include <QJsonArray>

#include "domain/data/RiskEvent.hpp"
#include "domain/data/MergedRiskEvent.hpp"
#include "domain/data/SectorSummary.hpp"
#include "domain/data/SectorSummaryData.hpp"
#include "domain/data/Track.hpp"
#include "domain/data/TrackData.hpp"

namespace Serializers
{
    QJsonObject toJson(const RiskEvent &event);
    QJsonObject toJson(const MergedRiskEvent &mergedEvent);
    QJsonObject toJson(const SectorSummary &sector);
    QJsonObject toJson(const SectorSummaryData &data);
    QJsonObject toJson(const Track &track);
    QJsonObject toJson(const TrackData &data);
}