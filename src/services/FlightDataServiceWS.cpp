#include <QNetworkRequest>
#include <QNetworkReply>

#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

#include <QRandomGenerator>

#include <QDebug>

#include "services/FlightDataServiceWS.hpp"
#include "domain/data/SectorSummary.hpp"
#include "domain/data/Track.hpp"
#include "domain/SectorStates.hpp"

#include "domain/serialization/Deserializers.hpp"
#include "domain/serialization/Serializers.hpp"

FlightDataServiceWS::FlightDataServiceWS(
    const QString &socketUrl,
    const QString &serverUrl,
    QObject *parent)
    : IFlightDataService(parent),
      _socketUrl(socketUrl),
      _serverUrl(serverUrl)
{
    _sectorSummaryData = std::make_unique<SectorSummaryData>();
    _trackData = std::make_unique<TrackData>();
    _riskEventData = std::make_unique<RiskEventData>();

    connect(&_socket, &QWebSocket::connected,
            this, &FlightDataServiceWS::onConnected);

    connect(&_socket, &QWebSocket::disconnected,
            this, &FlightDataServiceWS::onDisconnected);

    connect(&_socket, &QWebSocket::binaryMessageReceived,
            this, &FlightDataServiceWS::onMessageReceived);

    connect(&_socket, &QWebSocket::textMessageReceived,
            this, [this](const QString &message)
            { parseMessage(message.toUtf8()); });

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

    qDebug() << "Connecting to" << _socketUrl;

    _closedByUser = false;
    _socket.open(QUrl(_socketUrl));
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
        Deserializers::toSectorSummaryData(root));

    auto newTrackData = std::make_unique<TrackData>(
        Deserializers::toTrackData(root));

    auto newRiskEventData = std::make_unique<RiskEventData>(
        Deserializers::toRiskEventData(root));

    _sectorSummaryData = std::move(newSectorData);
    _trackData = std::move(newTrackData);
    _riskEventData = std::move(newRiskEventData);

    emit dataReloaded();
}

void FlightDataServiceWS::acknowledgeRiskEvents(const MergedRiskEvent &mergedEvent)
{
    QNetworkRequest request(QUrl(_serverUrl + "/risk-event/acknowledge"));


    qDebug() << "Server URL:" << request.url();

    request.setHeader(
        QNetworkRequest::ContentTypeHeader,
        "application/json");

    const QJsonObject body = Serializers::toJson(mergedEvent);

        qDebug() << "body:" << body;

    QNetworkReply *reply = _networkManager.put(
        request,
        QJsonDocument(body).toJson());

    connect(reply, &QNetworkReply::finished, this, [this, reply, mergedEvent]()
            {
        if (reply->error() != QNetworkReply::NoError)
        {
            qWarning() << "Acknowledge request failed:" << reply->errorString();
            emit acknowledgeFailed(mergedEvent);
            reply->deleteLater();
            return;
        }

        const int status =
            reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();

        qDebug() << "status:" << status;

        if (status == 204 || status == 200)
        {
            qDebug() << "[Service] Emitting success for sector" << mergedEvent.getSectorId();
            emit acknowledgeSucceeded(mergedEvent);
        }
        else
        {
            qDebug() << "[Service] Emitting failure for sector" << mergedEvent.getSectorId();
            emit acknowledgeFailed(mergedEvent);
        }

        reply->deleteLater(); });
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
