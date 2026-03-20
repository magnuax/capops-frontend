#pragma once
#include <map>
#include <vector>
#include <QObject>
#include <QFileSystemWatcher>
#include <QPoint>
#include <QSize>
#include <QString>

#include "services/interfaces/IFlightDataService.hpp"

class QPoint;
class QSize;
class QString;
class QFileSystemWatcher;

class FlightDataServiceJSON : public IFlightDataService
{
    Q_OBJECT

    using icao24_t = std::string;
    using sectorId_t = int;

public slots:
    void onFileChanged(const QString &path);

public:
    FlightDataServiceJSON(const QString &jsonPath, QObject *parent = nullptr);

    SectorSummaryData getSectorSummaryData() const override;
    RiskEventData getRiskEventData() const override;
    TrackData getTrackData() const override;

    void acknowledgeRiskEvents(const MergedRiskEvent &mergedEvent) override;

private:
    QString _jsonPath;
    QFileSystemWatcher _fileWatcher;

    bool loadFromJson();
    void reloadFromJson();

    RiskState parseRiskState(const QString &value) const;
    WeatherState parseWeatherState(const QString &value) const;
    TrafficState parseTrafficState(const QString &value) const;

    std::unique_ptr<SectorSummaryData> _sectorSummaryData;
    std::unique_ptr<TrackData> _trackData;

    std::vector<RiskState> _riskStates;
    std::vector<WeatherState> _weatherStates;
    std::vector<TrafficState> _trafficStates;

    int _rows;
    int _cols;
    std::vector<sectorId_t> _sectorIds;
    std::map<sectorId_t, std::vector<icao24_t>> _flightIds;
};
