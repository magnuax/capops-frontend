
#include "services/FlightDataServiceWS.hpp"
#include "services/WebSocketClient.hpp"

FlightDataServiceWS::FlightDataServiceWS(QObject *parent)
    : IFlightDataEvents(parent)
{
}

void FlightDataServiceWS::bindTo(WebSocketClient *client)
{
    connect(client, &WebSocketClient::sectorSummaryDataUpdated,
            this, &FlightDataServiceWS::updateSectorSummaryData);

    connect(client, &WebSocketClient::trackDataUpdated,
            this, &FlightDataServiceWS::updateTrackData);

    /*
    connect(client, &WebSocketClient::riskEventDataUpdated,
            this, &FlightDataServiceWS::updateRiskEventData);
    */
}

void FlightDataServiceWS::updateSectorSummaryData(const SectorSummaryData &data)
{
    *_sectorSummaryData = data;
    emit dataReloaded();
}

void FlightDataServiceWS::updateTrackData(const TrackData &data)
{
    *_trackData = data;
    emit dataReloaded();
}

SectorSummaryData FlightDataServiceWS::getSectorSummaryData() const
{
    return *_sectorSummaryData;
}

TrackData FlightDataServiceWS::getTrackData() const
{
    return *_trackData;
}