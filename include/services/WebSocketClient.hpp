#include <QObject>
#include <QWebSocket>
#include <vector>

#include "domain/SectorStates.hpp"

class QString;

class WebSocketClient : public QObject
{
    Q_OBJECT

signals:
    void sectorRiskReceived(int sectorId, RiskState risk);
    void sectorWeatherReceived(int sectorId, WeatherState weather);
    void sectorTrafficReceived(int sectorId, TrafficState traffic);
    void sectorFlightsReceived(int sectorId, std::vector<std::string> flightIds);

public:
    WebSocketClient(QObject *parent = nullptr);

    void connectToServer();

    void sendMessage(const QString &message);

private:
    QWebSocket _socket;
};