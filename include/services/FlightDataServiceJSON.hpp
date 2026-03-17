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

    using icao24_t = std::string;
    using sectorId_t = int;

public slots:
    void updateSectorRisk(int sectorId, RiskState risk);
    void updateSectorWeather(int sectorId, WeatherState weather);
    void updateSectorTraffic(int sectorId, TrafficState traffic);
    void updateSectorFlights(int sectorId, std::vector<icao24_t> flightIds);
    void onFileChanged(const QString &path);

public:
    FlightDataServiceJSON(const QString &jsonPath, QObject *parent = nullptr);

    SectorSummaryData getSectorSummaryData() const override;
    // RiskEventData getRiskEventData() const override;
    TrackData getTrackData() const override;

private:
    QString _jsonPath;
    QFileSystemWatcher _fileWatcher;

    bool loadFromJson();
    void reloadFromJson();

    RiskState parseRiskState(const QString &value) const;
    WeatherState parseWeatherState(const QString &value) const;
    TrafficState parseTrafficState(const QString &value) const;

    SectorSummaryData *_sectorSummaryData;
    TrackData *_trackData;

    std::vector<RiskState> _riskStates;
    std::vector<WeatherState> _weatherStates;
    std::vector<TrafficState> _trafficStates;

    int _rows;
    int _cols;
    std::vector<sectorId_t> _sectorIds;
    std::map<sectorId_t, std::vector<icao24_t>> _flightIds;
};
