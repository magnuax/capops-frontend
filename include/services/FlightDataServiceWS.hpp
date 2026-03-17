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

    using icao24_t = std::string;
    using sectorId_t = int;

public slots:
    void updateSectorRisk(int sectorId, RiskState risk);
    void updateSectorWeather(int sectorId, WeatherState weather);
    void updateSectorTraffic(int sectorId, TrafficState traffic);
    void updateSectorFlights(int sectorId, std::vector<icao24_t> flightIds);

public:
    FlightDataServiceWS(QObject *parent = nullptr);

    void bindTo(WebSocketClient *client);

    SectorSummaryData getSectorSummaryData() const override;
    // RiskEventData getRiskEventData() const override;
    TrackData getTrackData() const override;

private:
    int _rows;
    int _cols;

    std::vector<int> _sectorIds;

    std::map<sectorId_t, RiskState> _riskStates;
    std::map<sectorId_t, WeatherState> _weatherStates;
    std::map<sectorId_t, TrafficState> _trafficStates;
    std::map<sectorId_t, std::vector<icao24_t>> _flightIds;

    SectorSummaryData *_sectorSummaryData;
    TrackData *_trackData;
};
