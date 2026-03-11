#pragma once

#include <QWidget>

class StateGridPanel;
class AlertPanel;
class SectorDetailsPanel;

class ITileMapService;
class IFlightDataService;
class IFlightDataEvents;

class MainPage : public QWidget
{
    Q_OBJECT

public:
    explicit MainPage(
        IFlightDataService &dataService,
        IFlightDataEvents *dataEvents,
        ITileMapService *mapFetcher,
        QWidget *parent = nullptr);

private:
    QWidget *buildStateGrid();
    QWidget *buildAlertPanel();
    QWidget *buildSectorDetailsPanel();

    void wireConnections();

    StateGridPanel *_gridPanel = nullptr;
    AlertPanel *_alertPanel = nullptr;
    SectorDetailsPanel *_sectorDetailsPanel = nullptr;

    IFlightDataService &_dataService;
    IFlightDataEvents *_dataEvents = nullptr;
    ITileMapService *_mapFetcher = nullptr;

public slots:
    void requestMap(double latitude, double longitude, int zoomLevel);
};