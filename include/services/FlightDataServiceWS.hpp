#pragma once
#include <map>
#include <vector>
#include <QObject>
#include <QFileSystemWatcher>
#include <QPoint>
#include <QSize>
#include <QString>

#include "services/interfaces/IFlightDataService.hpp"
#include "services/interfaces/IFlightDataEvents.hpp"

class QPoint;
class QSize;
class QString;

class WebSocketClient;

class FlightDataServiceWS : public IFlightDataEvents, public IFlightDataService
{
    Q_OBJECT

public slots:
    void updateSectorRisk(int sectorId, RiskState risk);
    void updateSectorWeather(int sectorId, WeatherState weather);
    void updateSectorTraffic(int sectorId, TrafficState traffic);
    void updateSectorFlights(int sectorId, std::vector<std::string> flightIds);

public:
    FlightDataServiceWS(QObject *parent = nullptr);

    void bindTo(WebSocketClient *client);

    int getSectorId(int row, int col) const override;
    std::vector<int> getSectorIds() const override;

    int getRowCount() const override;
    int getColCount() const override;
    QSize getGridSize() const override;
    QPoint getSectorIndices(int sectorId) const override;

    RiskState getRisk(int sectorId) const override;
    WeatherState getWeather(int sectorId) const override;
    TrafficState getTraffic(int sectorId) const override;

    std::vector<std::string> getFlightIds(int sectorId) const override;

private:
    int _rows;
    int _cols;
    
    std::vector<int> _sectorIds;

    std::map<int, RiskState> _riskStates;
    std::map<int, WeatherState> _weatherStates;
    std::map<int, TrafficState> _trafficStates;
    std::map<int, std::vector<std::string>> _flightIds;
};
