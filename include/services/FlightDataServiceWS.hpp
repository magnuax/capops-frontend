#pragma once
#include <map>
#include <vector>
#include <QObject>
#include <QTimer>
#include <QPoint>
#include <QSize>
#include <QString>
#include <QAbstractSocket>
#include <QNetworkAccessManager>
#include <QWebSocket>

#include "services/interfaces/IFlightDataService.hpp"

class QPoint;
class QSize;
class QString;

class FlightDataServiceWS : public IFlightDataService
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
    explicit FlightDataServiceWS(
        const QString &socketUrl, 
        const QString &serverUrl, 
        QObject *parent = nullptr);

    void acknowledgeRiskEvents(const MergedRiskEvent &mergedEvent) override;

    SectorSummaryData getSectorSummaryData() const override;
    RiskEventData getRiskEventData() const override;
    TrackData getTrackData() const override;

    void connectToServer();
    void disconnectFromServer();

private:
    void scheduleReconnect();
    void parseMessage(const QByteArray &message);

    QString _socketUrl;
    QString _serverUrl;
    QTimer _reconnectTimer;
    QWebSocket _socket;
    QNetworkAccessManager _networkManager;

    int _retries = 0;
    bool _closedByUser = false;

    std::unique_ptr<SectorSummaryData> _sectorSummaryData;
    std::unique_ptr<RiskEventData> _riskEventData;
    std::unique_ptr<TrackData> _trackData;

    static constexpr int MAX_RECONNECT_MS = 15000;
    static constexpr int BASE_RECONNECT_MS = 500;
    static constexpr int MAX_RETRIES = 10;
};
