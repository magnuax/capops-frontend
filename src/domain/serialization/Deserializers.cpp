#include "domain/serialization/Deserializers.hpp"
#include "domain/SectorStates.hpp"

namespace Deserializers
{

    RiskEvent toRiskEvent(const QJsonObject &obj)
    {
        return RiskEvent(
            obj.value("riskEventId").toInt(),
            obj.value("sectorId").toInt(),
            obj.value("acknowledged").toBool(),
            riskStateFromString(obj.value("riskSeverity").toString()),
            QDateTime::fromString(obj.value("createdTimestamp").toString(), Qt::ISODate),
            QDateTime::fromString(obj.value("acknowledgedTimestamp").toString(), Qt::ISODate),
            obj.value("message").toString());
    }

    MergedRiskEvent toMergedRiskEvent(const QJsonObject &obj, const std::vector<RiskEvent> &events)
    {
        const int sectorId = obj.value("sectorId").toInt();

        std::vector<RiskEvent> mergedEvents;
        for (const QJsonValue &idVal : obj.value("riskEventIds").toArray())
        {
            const int id = idVal.toInt();
            auto it = std::find_if(events.begin(), events.end(),
                                   [id](const RiskEvent &e)
                                   { return e.getRiskEventId() == id; });
            if (it != events.end())
                mergedEvents.push_back(*it);
        }

        return MergedRiskEvent(sectorId, mergedEvents);
    }

    RiskEventData toRiskEventData(const QJsonObject &root)
    {
        const QJsonObject rawData = root.value("riskEventData").toObject();

        const int riskEventCount = rawData.value("riskEventCount").toInt();

        std::vector<RiskEvent> riskEvents;
        for (const QJsonValue &value : rawData.value("riskEvents").toArray())
            riskEvents.push_back(toRiskEvent(value.toObject()));

        std::vector<MergedRiskEvent> mergedRiskEvents;
        for (const QJsonValue &value : rawData.value("mergedRiskEvents").toArray())
            mergedRiskEvents.push_back(toMergedRiskEvent(value.toObject(), riskEvents));

        return RiskEventData(riskEventCount, riskEvents, mergedRiskEvents);
    }

    SectorSummary toSectorSummary(const QJsonObject &obj)
    {

        std::vector<QString> icao24List;

        for (const QJsonValue &value : obj.value("icao24List").toArray())
            icao24List.push_back(value.toString());

        return SectorSummary(
            obj.value("sectorId").toInt(),
            obj.value("row").toInt(),
            obj.value("column").toInt(),
            icao24List,
            computeTrafficState(
                obj.value("localAircraftCount").toInt(),
                obj.value("localAircraftBaseCapacity").toInt()),

            weatherStateFromString(obj.value("weatherSeverity").toString()),
            riskStateFromString(obj.value("riskSeverity").toString()));
    }

    SectorSummaryData toSectorSummaryData(const QJsonObject &root)
    {
        const QJsonObject data = root.value("sectorSummaryData").toObject();

        std::vector<SectorSummary> summaries;
        for (const QJsonValue &value : data.value("sectorSummaries").toArray())
            summaries.push_back(toSectorSummary(value.toObject()));

        return SectorSummaryData(
            data.value("rowsCount").toInt(),
            data.value("columnsCount").toInt(),
            data.value("minLatitude").toDouble(),
            data.value("maxLatitude").toDouble(),
            data.value("minLongitude").toDouble(),
            data.value("maxLongitude").toDouble(),
            summaries);
    }

    Track toTrack(const QJsonObject &obj)
    {
        const QJsonObject pos = obj.value("position").toObject();
        const QJsonObject vel = obj.value("velocity").toObject();

        return Track(
            obj.value("icao24").toString(),
            QDateTime::fromString(obj.value("timestamp").toString(), Qt::ISODate),
            {pos.value("latitudeDegrees").toDouble(),
             pos.value("longitudeDegrees").toDouble(),
             pos.value("altitudeFeet").toDouble()},
            {vel.value("groundSpeedKnots").toDouble(),
             vel.value("verticalSpeedFeetPerMinute").toDouble()},
            obj.value("headingDegrees").toDouble(),
            obj.value("groundTrackDegrees").toDouble());
    }

    TrackData toTrackData(const QJsonObject &root)
    {
        const QJsonObject rawData = root.value("trackData").toObject();

        std::vector<Track> tracks;
        for (const QJsonValue &value : rawData.value("tracks").toArray())
            tracks.push_back(toTrack(value.toObject()));

        return TrackData(
            rawData.value("totalAircraftCount").toInt(),
            rawData.value("coordinateSystem").toString(),
            tracks);
    }

}