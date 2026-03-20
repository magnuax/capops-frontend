#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QDebug>
#include <QRandomGenerator>

#include "services/FlightDataServiceWS.hpp"
#include "domain/data/SectorSummary.hpp"
#include "domain/data/Track.hpp"
#include "domain/SectorStates.hpp"

FlightDataServiceWS::FlightDataServiceWS(const QString &url, QObject *parent)
    : IFlightDataEvents(parent), _url(url)
{
    connect(&_socket, &QWebSocket::connected,
            this, &FlightDataServiceWS::onConnected);

    connect(&_socket, &QWebSocket::disconnected,
            this, &FlightDataServiceWS::onDisconnected);

    connect(&_socket, &QWebSocket::binaryMessageReceived,
            this, &FlightDataServiceWS::onMessageReceived);

    connect(&_socket, &QWebSocket::errorOccurred,
            this, &FlightDataServiceWS::onError);

    _reconnectTimer.setSingleShot(true);

    connect(&_reconnectTimer, &QTimer::timeout,
            this, &FlightDataServiceWS::connectToServer);
}

void FlightDataServiceWS::connectToServer()
{
    if (_socket.state() == QAbstractSocket::ConnectedState ||
        _socket.state() == QAbstractSocket::ConnectingState)
        return;

    _closedByUser = false;
    _socket.open(QUrl(_url));
}

void FlightDataServiceWS::disconnectFromServer()
{
    _closedByUser = true;
    _reconnectTimer.stop();
    _socket.close();
}

void FlightDataServiceWS::onConnected()
{
    qDebug() << "WebSocket connected";
    _retries = 0;
}

void FlightDataServiceWS::onDisconnected()
{
    qDebug() << "WebSocket disconnected";
    if (!_closedByUser)
        scheduleReconnect();
}

void FlightDataServiceWS::onError(QAbstractSocket::SocketError error)
{
    qWarning() << "WebSocket error:" << _socket.errorString();
}

void FlightDataServiceWS::onMessageReceived(const QByteArray &message)
{
    parseMessage(message);
}

void FlightDataServiceWS::scheduleReconnect()
{
    _retries++;

    const int mult = 1 << std::min(_retries - 1, MAX_RETRIES); // Exponential backoff (1 << N) = 2^N w/ left bit shift
    const int base = std::min(MAX_RECONNECT_MS, BASE_RECONNECT_MS * mult);
    const int jitter = QRandomGenerator::global()->bounded(300);

    qDebug() << "Reconnecting in" << base + jitter << "ms (attempt" << _retries << ")";

    _reconnectTimer.start(base + jitter);
}

void FlightDataServiceWS::parseMessage(const QByteArray &message)
{
    QJsonParseError error;
    const QJsonDocument doc = QJsonDocument::fromJson(message, &error);

    if (error.error != QJsonParseError::NoError)
    {
        qWarning() << "Failed to parse WebSocket message:" << error.errorString();
        return;
    }

    const QJsonObject root = doc.object();

    auto newSectorData = std::make_unique<SectorSummaryData>(
        parseSectorSummaryData(root));

    auto newTrackData = std::make_unique<TrackData>(
        parseTrackData(root));

    auto newRiskEventData = std::make_unique<RiskEventData>(
        parseRiskEventData(root));

    _sectorSummaryData = std::move(newSectorData);
    _trackData = std::move(newTrackData);
    _riskEventData = std::move(newRiskEventData);

    emit dataReloaded();
}

SectorSummaryData FlightDataServiceWS::parseSectorSummaryData(const QJsonObject &root) const
{
    const QJsonObject data = root.value("sectorSummaryData").toObject();

    const int rows = data.value("rowsCount").toInt();
    const int cols = data.value("columnsCount").toInt();
    const double minLon = data.value("minLongitude").toDouble();
    const double maxLon = data.value("maxLongitude").toDouble();
    const double minLat = data.value("minLatitude").toDouble();
    const double maxLat = data.value("maxLatitude").toDouble();

    std::vector<SectorSummary> summaries;
    const QJsonArray sectorArray = data.value("sectorSummaries").toArray();

    for (const QJsonValue &value : sectorArray)
    {
        const QJsonObject sector = value.toObject();

        const int id = sector.value("sectorId").toInt();
        const int row = sector.value("row").toInt();
        const int col = sector.value("column").toInt();

        const RiskState risk = riskStateFromString(sector.value("riskSeverity").toString());
        const WeatherState weather = weatherStateFromString(sector.value("weatherSeverity").toString());
        const TrafficState traffic = computeTrafficState(
            sector.value("aircraftCount").toInt(),
            sector.value("baseCapacity").toInt());

        summaries.push_back(SectorSummary(id, row, col, {}, traffic, weather, risk));
    }

    return SectorSummaryData(rows, cols, minLon, maxLon, minLat, maxLat, summaries);
}

TrackData FlightDataServiceWS::parseTrackData(const QJsonObject &root) const
{
    const QJsonObject rawData = root.value("trackData").toObject();

    const int totalCount = rawData.value("totalAircraftCount").toInt();
    const QString coordSystem = rawData.value("coordinateSystem").toString();

    std::vector<Track> tracks;
    const QJsonArray trackArray = rawData.value("tracks").toArray();

    for (const QJsonValue &value : trackArray)
    {
        const QJsonObject track = value.toObject();

        const QString icao24 = track.value("icao24").toString();
        const QDateTime timestamp = QDateTime::fromString(
            track.value("timestamp").toString(), Qt::ISODate);

        const QJsonObject pos = track.value("position").toObject();
        const QJsonObject vel = track.value("velocity").toObject();

        const std::array<double, 3> position = {
            pos.value("latitudeDegrees").toDouble(),
            pos.value("longitudeDegrees").toDouble(),
            pos.value("altitudeFeet").toDouble()};

        const std::array<double, 2> velocity = {
            vel.value("groundSpeedKnots").toDouble(),
            vel.value("verticalSpeedFeetPerMinute").toDouble()};

        const double heading = track.value("headingDegrees").toDouble();
        const double groundTrack = track.value("groundTrackDegrees").toDouble();

        tracks.push_back(Track(icao24, timestamp, position, velocity, heading, groundTrack));
    }

    return TrackData(totalCount, coordSystem, tracks);
}

RiskEventData FlightDataServiceWS::parseRiskEventData(const QJsonObject &root) const
{
    const QJsonObject rawData = root.value("riskEventData").toObject();

    const int riskEventCount = rawData.value("riskEventCount").toInt();

    std::vector<RiskEvent> riskEvents;
    const QJsonArray riskEventArray = rawData.value("riskEvents").toArray();

    for (const QJsonValue &value : riskEventArray)
    {
        const QJsonObject obj = value.toObject();

        const int riskEventId = obj.value("riskEventId").toInt();
        const RiskState riskState = riskStateFromString(obj.value("riskSeverity").toString());
        const int sectorId = obj.value("sectorId").toInt();
        const QDateTime createdTimestamp = QDateTime::fromString(
            obj.value("createdTimestamp").toString(), Qt::ISODate);
        const QDateTime acknowledgedTimestamp = QDateTime::fromString(
            obj.value("acknowledgedTimestamp").toString(), Qt::ISODate);
        const QString message = obj.value("message").toString();
        const bool acknowledged = obj.value("acknowledged").toBool();

        riskEvents.push_back(RiskEvent(
            riskEventId,
            sectorId,
            acknowledged,
            riskState,
            createdTimestamp,
            acknowledgedTimestamp,
            message));
    }

    std::vector<MergedRiskEvent> mergedRiskEvents;
    const QJsonArray mergedArray = rawData.value("mergedRiskEvents").toArray();

    for (const QJsonValue &value : mergedArray)
    {
        const QJsonObject obj = value.toObject();

        const int sectorId = obj.value("sectorId").toInt();
        const QString summaryMessage = obj.value("summaryMessage").toString();
        const QString lastMessage = obj.value("lastMessage").toString();

        std::vector<RiskEvent> events;
        for (const QJsonValue &idVal : obj.value("riskEventIds").toArray())
            events.push_back(riskEvents[idVal.toInt()]);

        mergedRiskEvents.push_back(MergedRiskEvent(sectorId, events));
    }

    return RiskEventData(riskEventCount, riskEvents, mergedRiskEvents);
}

SectorSummaryData FlightDataServiceWS::getSectorSummaryData() const
{

    return *_sectorSummaryData;
}

TrackData FlightDataServiceWS::getTrackData() const
{
    return *_trackData;
}

RiskEventData FlightDataServiceWS::getRiskEventData() const
{
    return *_riskEventData;
}
