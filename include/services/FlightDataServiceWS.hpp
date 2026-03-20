#pragma once
#include <map>
#include <vector>
#include <QObject>
#include <QTimer>
#include <QPoint>
#include <QSize>
#include <QString>
#include <QAbstractSocket>
#include <QWebSocket>

#include "services/interfaces/IFlightDataService.hpp"
#include "services/interfaces/IFlightDataEvents.hpp"

class QPoint;
class QSize;
class QString;

class FlightDataServiceWS : public IFlightDataEvents, public IFlightDataService
{
    Q_OBJECT

    using icao24_t = std::string;
    using sectorId_t = int;

private slots:
    void onConnected();
    void onDisconnected();
    void onMessageReceived(const QByteArray &message);
    void onError(QAbstractSocket::SocketError error);

public:
    explicit FlightDataServiceWS(const QString &url, QObject *parent = nullptr);

    SectorSummaryData getSectorSummaryData() const override;
    RiskEventData getRiskEventData() const override;
    TrackData getTrackData() const override;

    void connectToServer();
    void disconnectFromServer();

private:
    void scheduleReconnect();
    void parseMessage(const QByteArray &message);

    SectorSummaryData parseSectorSummaryData(const QJsonObject &root) const;
    RiskEventData parseRiskEventData(const QJsonObject &root) const;
    TrackData parseTrackData(const QJsonObject &root) const;

    QString _url;
    QWebSocket _socket;
    QTimer _reconnectTimer;

    int _retries = 0;
    bool _closedByUser = false;

    std::unique_ptr<SectorSummaryData> _sectorSummaryData;
    std::unique_ptr<RiskEventData> _riskEventData;
    std::unique_ptr<TrackData> _trackData;

    static constexpr int MAX_RECONNECT_MS = 15000;
    static constexpr int BASE_RECONNECT_MS = 500;
    static constexpr int MAX_RETRIES = 10;
};
