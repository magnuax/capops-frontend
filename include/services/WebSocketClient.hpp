#include <QObject>
#include <QWebSocket>
#include <vector>

#include <QJsonObject>
#include <QUrl>

#include "domain/SectorStates.hpp"

class QString;

class WebSocketClient : public QObject
{
    Q_OBJECT

signals:
    void sectorRiskUpdated(int sectorId, RiskState risk);
    void sectorWeatherUpdated(int sectorId, WeatherState weather);
    void sectorTrafficUpdated(int sectorId, TrafficState traffic);
    void sectorFlightsUpdated(int sectorId, std::vector<std::string> flightIds);

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