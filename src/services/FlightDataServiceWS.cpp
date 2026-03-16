
#include "services/FlightDataServiceWS.hpp"
#include "services/WebSocketClient.hpp"

FlightDataServiceWS::FlightDataServiceWS(QObject *parent)
    : IFlightDataEvents(parent)
{
}

void FlightDataServiceWS::bindTo(WebSocketClient *client)
{
    connect(client, &WebSocketClient::sectorRiskReceived,
            this, &FlightDataServiceWS::updateSectorRisk);

    connect(client, &WebSocketClient::sectorWeatherReceived,
            this, &FlightDataServiceWS::updateSectorWeather);

    connect(client, &WebSocketClient::sectorTrafficReceived,
            this, &FlightDataServiceWS::updateSectorTraffic);

    connect(client, &WebSocketClient::sectorFlightsReceived,
            this, &FlightDataServiceWS::updateSectorFlights);
}

int FlightDataServiceWS::getSectorId(int row, int col) const
{
    // Implementation to retrieve sector ID based on row and column
    return -1;
}

std::vector<int> FlightDataServiceWS::getSectorIds() const
{
    // Implementation to retrieve all sector IDs
    return {};
}

int FlightDataServiceWS::getRowCount() const
{
    // Implementation to retrieve the number of rows in the grid
    return -1;
}

int FlightDataServiceWS::getColCount() const
{
    // Implementation to retrieve the number of columns in the grid
    return -1;
}

QSize FlightDataServiceWS::getGridSize() const
{
    // Implementation to retrieve the size of the grid
    return QSize(-1, -1);
}

QPoint FlightDataServiceWS::getSectorIndices(int sectorId) const
{
    // Implementation to retrieve the row and column indices for a given sector ID
    return QPoint(-1, -1);
}

RiskState FlightDataServiceWS::getRisk(int sectorId) const
{
    // Implementation to retrieve the risk state for a given sector ID
    return RiskState::NORMAL;
}

WeatherState FlightDataServiceWS::getWeather(int sectorId) const
{
    // Implementation to retrieve the weather state for a given sector ID
    return WeatherState::OK;
}

TrafficState FlightDataServiceWS::getTraffic(int sectorId) const
{
    // Implementation to retrieve the traffic state for a given sector ID
    return TrafficState::LIGHT;
}

std::vector<std::string> FlightDataServiceWS::getFlightIds(int sectorId) const
{
    // Implementation to retrieve the list of flight IDs for a given sector ID
    return {};
}

void FlightDataServiceWS::updateSectorRisk(int sectorId, RiskState risk)
{
    if (sectorId >= 0)
        _riskStates.at(sectorId) = risk;
}

void FlightDataServiceWS::updateSectorWeather(int sectorId, WeatherState weather)
{
    if (sectorId >= 0)
        _weatherStates.at(sectorId) = weather;
}

void FlightDataServiceWS::updateSectorTraffic(int sectorId, TrafficState traffic)
{
    if (sectorId >= 0)
        _trafficStates.at(sectorId) = traffic;
}

void FlightDataServiceWS::updateSectorFlights(int sectorId, std::vector<std::string> flightIds)
{
    if (sectorId >= 0)
        _flightIds.at(sectorId) = flightIds;
}