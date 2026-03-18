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
    void updateSectorSummaryData(const SectorSummaryData &data);
    // void updateRiskEventData(const RiskEventData &data);
    void updateTrackData(const TrackData &data);

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

    SectorSummaryData *_sectorSummaryData;
    TrackData *_trackData;
};
