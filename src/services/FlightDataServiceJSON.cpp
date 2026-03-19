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

        QJsonArray trackArray = sector.value("tracks").toArray();

        for (const QJsonValue &value : trackArray)
        {
            const QJsonObject track = value.toObject();

            const QString icao24 = track.value("icao24").toString();
            aircraftIds.push_back(icao24);

            const QDateTime timestamp = QDateTime::fromSecsSinceEpoch(track.value("timestamp").toInt());

            const std::array<double, 3> position = {
                track.value("position").toObject().value("x").toDouble(),
                track.value("position").toObject().value("y").toDouble(),
                track.value("altitude").toDouble()};

            const std::array<double, 2> velocity = {
                track.value("speed").toDouble(),
                track.value("speed").toDouble()};

            const double heading = track.value("heading").toDouble();
            const double groundTrack = track.value("groundTrack").toDouble();

            tracks.push_back(Track(icao24, timestamp, position, velocity, heading, groundTrack));
        }

        totalAircraftCount += aircraftIds.size();

        const int id = sector.value("id").toInt(-1);
        const int row = sector.value("row").toInt();
        const int col = sector.value("col").toInt();
        const int count = aircraftIds.size();
        const int eff = sector.value("effective_capacity").toInt();

        const TrafficState traffic = parseTrafficState(sector.value("traffic").toString());
        const WeatherState weather = parseWeatherState(sector.value("weather").toString());
        const RiskState risk = parseRiskState(sector.value("risk").toString());

        sectorSummaries.push_back(SectorSummary(id, row, col, aircraftIds, traffic, weather, risk));
    }

    _rows = root.value("rows").toInt();
    _cols = root.value("cols").toInt();

    double minLat = root.value("minLat").toDouble(-1);
    double maxLat = root.value("maxLat").toDouble(-1);
    double minLon = root.value("minLon").toDouble(-1);
    double maxLon = root.value("maxLon").toDouble(-1);

    _sectorSummaryData = std::make_unique<SectorSummaryData>(_rows, _cols, minLat, maxLat, minLon, maxLon, sectorSummaries);
    _trackData = std::make_unique<TrackData>(totalAircraftCount, "WGS84", tracks);

    return true;
}

RiskState FlightDataServiceJSON::parseRiskState(const QString &value) const
{
    if (value == "NORMAL")
        return RiskState::NORMAL;
    if (value == "AT_RISK")
        return RiskState::AT_RISK;
    if (value == "CONGESTED")
        return RiskState::CONGESTED;
    return RiskState::UNKNOWN;
}

WeatherState FlightDataServiceJSON::parseWeatherState(const QString &value) const
{
    if (value == "OK")
        return WeatherState::OK;
    if (value == "DEGRADED")
        return WeatherState::DEGRADED;
    if (value == "SEVERE")
        return WeatherState::SEVERE;
    if (value == "EXTREME")
        return WeatherState::EXTREME;
    return WeatherState::UNKNOWN;
}

TrafficState FlightDataServiceJSON::parseTrafficState(const QString &value) const
{
    if (value == "LIGHT")
        return TrafficState::LIGHT;
    if (value == "MODERATE")
        return TrafficState::MODERATE;
    if (value == "HEAVY")
        return TrafficState::HEAVY;
    return TrafficState::LIGHT;
}

SectorSummaryData FlightDataServiceJSON::getSectorSummaryData() const
{
    return *_sectorSummaryData;
}

TrackData FlightDataServiceJSON::getTrackData() const
{
    return *_trackData;
}