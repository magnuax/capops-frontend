
#include "services/FlightDataServiceWS.hpp"
#include "services/WebSocketClient.hpp"

FlightDataServiceWS::FlightDataServiceWS(QObject *parent)
    : IFlightDataEvents(parent)
{
}

void FlightDataServiceWS::bindTo(WebSocketClient *client)
{
    connect(client, &WebSocketClient::sectorRiskUpdated,
            this, &FlightDataServiceWS::updateSectorRisk);

    connect(client, &WebSocketClient::sectorWeatherUpdated,
            this, &FlightDataServiceWS::updateSectorWeather);

    connect(client, &WebSocketClient::sectorTrafficUpdated,
            this, &FlightDataServiceWS::updateSectorTraffic);

    connect(client, &WebSocketClient::sectorFlightsUpdated,
            this, &FlightDataServiceWS::updateSectorFlights);
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

SectorSummaryData FlightDataServiceWS::getSectorSummaryData() const
{
    return *_sectorSummaryData;
}

TrackData FlightDataServiceWS::getTrackData() const
{
    return *_trackData;
}