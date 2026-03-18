#include <QObject>
#include <QWebSocket>
#include <vector>

#include <QJsonObject>
#include <QUrl>

#include "domain/SectorStates.hpp"
#include "domain/data/SectorSummaryData.hpp"
#include "domain/data/RiskEventData.hpp"
#include "domain/data/TrackData.hpp"

class QString;

class WebSocketClient : public QObject
{
    Q_OBJECT

signals:
    void sectorSummaryDataUpdated(const SectorSummaryData &data);
    // void riskEventDataUpdated(const RiskEventData &data);
    void trackDataUpdated(const TrackData &data);

    void connected();
    void disconnected();
    void errorOccurred(const QString &message);

private slots:
    void onMessageReceived(const QString &message);
    void onConnected();
    void onDisconnected();
    void onError(QAbstractSocket::SocketError error);

public:
    WebSocketClient(QObject *parent = nullptr);

    void connectToServer(const QUrl &url);
    void disconnect();
    bool isConnected() const;

private:
    void parseMessage(const QJsonObject &json);

    QWebSocket _socket;
};