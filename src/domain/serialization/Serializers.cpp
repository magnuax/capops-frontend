#include "domain/serialization/Serializers.hpp"
#include "domain/SectorStates.hpp"

namespace Serializers
{

    QJsonObject toJson(const RiskEvent &event)
    {
        QJsonObject obj;
        obj["riskEventId"] = event.getRiskEventId();
        obj["sectorId"] = event.getSectorId();
        obj["acknowledged"] = event.getAcknowledged();
        obj["riskSeverity"] = toString(event.getRiskState());
        obj["createdTimestamp"] = event.getCreatedTimestamp().toString(Qt::ISODate);
        obj["acknowledgedTimestamp"] = event.getAcknowledgedTimestamp().toString(Qt::ISODate);
        obj["message"] = event.getMessage();
        return obj;
    }

    QJsonObject toJson(const MergedRiskEvent &mergedEvent)
    {
        QJsonObject obj;
        obj["sectorId"] = mergedEvent.getSectorId();
        obj["summaryMessage"] = mergedEvent.getSummaryMessage();
        obj["lastMessage"] = mergedEvent.getLastMessage();
        obj["checksum"] = "PLACEHOLDER"; // toString(mergedEvent.getLastRiskState());

        QJsonArray events;
        for (const RiskEvent &event : mergedEvent.getRiskEvents())
            events.append(event.getRiskEventId());

        obj["riskEventIds"] = events;
        return obj;
    }

    QJsonObject toJson(const SectorSummary &sector)
    {
        QStringList icao24List;
        for (const QString &icao24 : sector.getAircraftIds())
            icao24List.append(icao24);

        QJsonObject obj;
        obj["sectorId"] = sector.getSectorId();
        obj["row"] = sector.getRow();
        obj["column"] = sector.getCol();
        obj["riskSeverity"] = toString(sector.getRiskState());
        obj["weatherSeverity"] = toString(sector.getWeatherState());
        obj["icao24List"] = QJsonArray::fromStringList(icao24List);
        return obj;
    }

    QJsonObject toJson(const SectorSummaryData &data)
    {
        QJsonObject obj;
        obj["rowsCount"] = data.getRowCount();
        obj["columnsCount"] = data.getColCount();
        obj["minLongitude"] = data.getMinLon();
        obj["maxLongitude"] = data.getMaxLon();
        obj["minLatitude"] = data.getMinLat();
        obj["maxLatitude"] = data.getMaxLat();

        QJsonArray sectors;
        for (const SectorSummary &sector : data.getSectorSummaries())
            sectors.append(toJson(sector));

        obj["sectorSummaries"] = sectors;
        return obj;
    }

    QJsonObject toJson(const Track &track)
    {
        QJsonObject position;
        position["latitudeDegrees"] = track.getPosition()[0];
        position["longitudeDegrees"] = track.getPosition()[1];
        position["altitudeFeet"] = track.getPosition()[2];

        QJsonObject velocity;
        velocity["groundSpeedKnots"] = track.getVelocity()[0];
        velocity["verticalSpeedFeetPerMinute"] = track.getVelocity()[1];

        QJsonObject obj;
        obj["icao24"] = track.getIcao24();
        obj["timestamp"] = track.getTimestamp().toString(Qt::ISODate);
        obj["position"] = position;
        obj["velocity"] = velocity;
        obj["headingDegrees"] = track.getHeading();
        obj["groundTrackDegrees"] = track.getGroundTrack();
        return obj;
    }

    QJsonObject toJson(const TrackData &data)
    {
        QJsonObject obj;
        obj["totalAircraftCount"] = data.getTotalAircraftCount();
        obj["coordinateSystem"] = data.getCoordinateSystem();

        QJsonArray tracks;
        for (const Track &track : data.getTracks())
            tracks.append(toJson(track));

        obj["tracks"] = tracks;
        return obj;
    }

} // namespace Serializers