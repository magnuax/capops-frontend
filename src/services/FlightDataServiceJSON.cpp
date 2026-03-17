#include <QObject>
#include <QFileSystemWatcher>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QDebug>
#include <QFile>
#include <QPoint>
#include <QSize>
#include <QString>

#include "services/FlightDataServiceJSON.hpp"

#include "domain/data/SectorSummaryData.hpp"
#include "domain/data/SectorSummary.hpp"
#include "domain/data/Track.hpp"
#include "domain/data/TrackData.hpp"

FlightDataServiceJSON::FlightDataServiceJSON(const QString &jsonPath, QObject *parent)
    : IFlightDataEvents(parent), _jsonPath(jsonPath)
{
    loadFromJson();

    _fileWatcher.addPath(_jsonPath);

    connect(&_fileWatcher, &QFileSystemWatcher::fileChanged,
            this, &FlightDataServiceJSON::onFileChanged);
}

void FlightDataServiceJSON::onFileChanged(const QString &path)
{
    reloadFromJson();

    if (!_fileWatcher.files().contains(path))
        _fileWatcher.addPath(path);
}

void FlightDataServiceJSON::reloadFromJson()
{
    if (loadFromJson())
        emit dataReloaded();
}

bool FlightDataServiceJSON::loadFromJson()
{
    QFile file(_jsonPath);
    file.open(QIODevice::ReadOnly);
    const QByteArray data = file.readAll();
    file.close();

    QJsonParseError error;
    const QJsonDocument doc = QJsonDocument::fromJson(data, &error);
    const QJsonObject root = doc.object();

    const QJsonArray sectors = root.value("sectors").toArray();
    const int numSectors = sectors.size();

    std::vector<int> sectorIds;
    std::vector<RiskState> riskStates;
    std::vector<WeatherState> weatherStates;
    std::vector<TrafficState> trafficStates;
    std::map<int, std::vector<std::string>> flightIds;

    sectorIds.reserve(numSectors);
    riskStates.reserve(numSectors);
    weatherStates.reserve(numSectors);
    trafficStates.reserve(numSectors);

    int totalAircraftCount = 0;
    std::vector<SectorSummary> sectorSummaries;
    std::vector<Track> tracks;

    for (const QJsonValue &value : sectors)
    {
        const QJsonObject sector = value.toObject();

        std::vector<QString> aircraftIds;

        for (const QJsonValue &aircraft : sector.value("aircrafts").toArray())
        {
            const QString icao24 = aircraft.toString();
            aircraftIds.push_back(icao24);
        }

        const int id = sector.value("id").toInt(-1);
        const int row = sector.value("row").toInt();
        const int col = sector.value("col").toInt();
        const int count = aircraftIds.size();
        const int eff = sector.value("effective_capacity").toInt();

        const TrafficState traffic = parseTrafficState(sector.value("traffic").toString());
        const WeatherState weather = parseWeatherState(sector.value("weather").toString());
        const RiskState risk = parseRiskState(sector.value("risk").toString());

        sectorSummaries.push_back(SectorSummary(id, row, col, aircraftIds, traffic, weather, risk));

        for (const auto &icao24 : aircraftIds)
        {
            tracks.push_back(
                Track(icao24, QDateTime{}, {-1.0, -1.0, -1.0}, {-1.0, -1.0}, -1.0, -1.0));
        }

        totalAircraftCount += count;
    }

    _rows = root.value("rows").toInt();
    _cols = root.value("cols").toInt();
    _sectorSummaryData = new SectorSummaryData(_rows, _cols, -1, -1, -1, -1, sectorSummaries);

    _trackData = new TrackData(totalAircraftCount, "WGS84", tracks);

    return true;
}

RiskState FlightDataServiceJSON::parseRiskState(const QString &value) const
{
    if (value == "NORMAL")
        return NORMAL;
    if (value == "AT_RISK")
        return AT_RISK;
    if (value == "CONGESTED")
        return CONGESTED;
    return NORMAL;
}

WeatherState FlightDataServiceJSON::parseWeatherState(const QString &value) const
{
    if (value == "OK")
        return OK;
    if (value == "DEGRADED")
        return DEGRADED;
    if (value == "SEVERE")
        return SEVERE;
    if (value == "EXTREME")
        return EXTREME;
    return OK;
}

TrafficState FlightDataServiceJSON::parseTrafficState(const QString &value) const
{
    if (value == "LIGHT")
        return LIGHT;
    if (value == "MODERATE")
        return MODERATE;
    if (value == "HEAVY")
        return HEAVY;
    return LIGHT;
}

void FlightDataServiceJSON::updateSectorRisk(int sectorId, RiskState risk)
{
    if (sectorId >= 0)
        _riskStates[sectorId] = risk;
}

void FlightDataServiceJSON::updateSectorWeather(int sectorId, WeatherState weather)
{
    if (sectorId >= 0)
        _weatherStates[sectorId] = weather;
}

void FlightDataServiceJSON::updateSectorTraffic(int sectorId, TrafficState traffic)
{
    if (sectorId >= 0)
        _trafficStates[sectorId] = traffic;
}

void FlightDataServiceJSON::updateSectorFlights(int sectorId, std::vector<std::string> flightIds)
{
    if (sectorId >= 0)
        _flightIds[sectorId] = flightIds;
}

SectorSummaryData FlightDataServiceJSON::getSectorSummaryData() const
{
    return *_sectorSummaryData;
}

TrackData FlightDataServiceJSON::getTrackData() const
{
    return *_trackData;
}