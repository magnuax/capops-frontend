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

#include "services/FlightDataServiceTest.hpp"

FlightDataServiceTest::FlightDataServiceTest(const QString &jsonPath, QObject *parent)
    : IFlightDataEvents(parent), _jsonPath(jsonPath)
{
    loadFromJson();

    _fileWatcher.addPath(_jsonPath);

    connect(&_fileWatcher, &QFileSystemWatcher::fileChanged,
            this, &FlightDataServiceTest::onFileChanged);
}

void FlightDataServiceTest::onFileChanged(const QString &path)
{
    reloadFromJson();

    if (!_fileWatcher.files().contains(path))
    {
        _fileWatcher.addPath(path);
    }
    return;
}

void FlightDataServiceTest::reloadFromJson()
{
    if (loadFromJson())
    {
        emit dataReloaded();
    }
    return;
}

bool FlightDataServiceTest::loadFromJson()
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
    std::map<int, std::vector<int>> flightIds;

    sectorIds.reserve(numSectors);
    riskStates.reserve(numSectors);
    weatherStates.reserve(numSectors);
    trafficStates.reserve(numSectors);

    for (const QJsonValue &value : sectors)
    {
        const QJsonObject sector = value.toObject();

        const int id = sector.value("id").toInt(-1);

        sectorIds.push_back(id);
        riskStates.push_back(parseRiskState(sector.value("risk").toString()));
        weatherStates.push_back(parseWeatherState(sector.value("weather").toString()));
        trafficStates.push_back(parseTrafficState(sector.value("traffic").toString()));

        std::vector<int> flights;
        const QJsonArray flightsArray = sector.value("flights").toArray();
        flights.reserve(flightsArray.size());

        for (const QJsonValue &flightValue : flightsArray)
        {
            flights.push_back(flightValue.toInt());
        }
        
        flightIds[id] = std::move(flights);
    }

    _rows = root.value("rows").toInt();
    _cols = root.value("cols").toInt();
    _sectorIds = std::move(sectorIds);
    _riskStates = std::move(riskStates);
    _weatherStates = std::move(weatherStates);
    _trafficStates = std::move(trafficStates);
    _flightIds = std::move(flightIds);

    return true;
}

RiskState FlightDataServiceTest::parseRiskState(const QString &value) const
{
    if (value == "NORMAL")
        return NORMAL;
    if (value == "AT_RISK")
        return AT_RISK;
    if (value == "CONGESTED")
        return CONGESTED;
    return NORMAL;
}

WeatherState FlightDataServiceTest::parseWeatherState(const QString &value) const
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

TrafficState FlightDataServiceTest::parseTrafficState(const QString &value) const
{
    if (value == "LIGHT")
        return LIGHT;
    if (value == "MODERATE")
        return MODERATE;
    if (value == "HEAVY")
        return HEAVY;
    return LIGHT;
}

void FlightDataServiceTest::updateSectorRisk(int sectorId, RiskState risk)
{
    if (sectorId >= 0)
    {
        _riskStates[sectorId] = risk;
    }

    return;
}
void FlightDataServiceTest::updateSectorWeather(int sectorId, WeatherState weather)
{
    if (sectorId >= 0)
    {
        _weatherStates[sectorId] = weather;
    }

    return;
}

void FlightDataServiceTest::updateSectorTraffic(int sectorId, TrafficState traffic)
{
    if (sectorId >= 0)
    {
        _trafficStates[sectorId] = traffic;
    }

    return;
}
void FlightDataServiceTest::updateSectorFlights(int sectorId, std::vector<int> flightIds)
{
    if (sectorId >= 0)
    {
        _flightIds[sectorId] = flightIds;
    }

    return;
}

int FlightDataServiceTest::getRowCount() const
{
    return _rows;
}

int FlightDataServiceTest::getColCount() const
{
    return _cols;
}

QSize FlightDataServiceTest::getGridSize() const
{
    return QSize(getColCount(), getRowCount());
}

RiskState FlightDataServiceTest::getRisk(int sectorId) const
{
    return _riskStates[sectorId];
}

WeatherState FlightDataServiceTest::getWeather(int sectorId) const
{
    return _weatherStates[sectorId];
}

TrafficState FlightDataServiceTest::getTraffic(int sectorId) const
{
    return _trafficStates[sectorId];
}

QPoint FlightDataServiceTest::getSectorIndices(int sectorId) const
{
    const int row = sectorId / getColCount();
    const int col = sectorId % getColCount();
    return {row, col};
}

int FlightDataServiceTest::getSectorId(int row, int col) const
{
    return _sectorIds[row * getColCount() + col];
}

std::vector<int> FlightDataServiceTest::getSectorIds() const
{
    return _sectorIds;
}

std::vector<int> FlightDataServiceTest::getFlightIds(int sectorId) const
{
    return _flightIds.at(sectorId);
}