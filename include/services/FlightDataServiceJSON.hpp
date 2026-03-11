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
class QFileSystemWatcher;

class FlightDataServiceJSON : public IFlightDataEvents, public IFlightDataService
{
    Q_OBJECT

public slots:
    void updateSectorRisk(int sectorId, RiskState risk);
    void updateSectorWeather(int sectorId, WeatherState weather);
    void updateSectorTraffic(int sectorId, TrafficState traffic);
    void updateSectorFlights(int sectorId, std::vector<int> flightIds);
    void onFileChanged(const QString &path);

public:
    FlightDataServiceJSON(const QString &jsonPath, QObject *parent = nullptr);

    int getSectorId(int row, int col) const override;
    std::vector<int> getSectorIds() const override;

    int getRowCount() const override;
    int getColCount() const override;
    QSize getGridSize() const override;
    QPoint getSectorIndices(int sectorId) const override;

    RiskState getRisk(int sectorId) const override;
    WeatherState getWeather(int sectorId) const override;
    TrafficState getTraffic(int sectorId) const override;

    std::vector<int> getFlightIds(int sectorId) const override;

private:
    QString _jsonPath;
    QFileSystemWatcher _fileWatcher;

    bool loadFromJson();
    void reloadFromJson();

    RiskState parseRiskState(const QString &value) const;
    WeatherState parseWeatherState(const QString &value) const;
    TrafficState parseTrafficState(const QString &value) const;

    std::vector<RiskState> _riskStates;
    std::vector<WeatherState> _weatherStates;
    std::vector<TrafficState> _trafficStates;

    int _rows;
    int _cols;
    std::vector<int> _sectorIds;
    std::map<int, std::vector<int>> _flightIds;
};
