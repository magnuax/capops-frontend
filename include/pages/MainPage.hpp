#pragma once

#include <QWidget>

class StateGridPanel;
class AlertPanel;
class SectorDetailsPanel;

class ITileMapService;
class IFlightDataService;

class MainPage : public QWidget
{
    Q_OBJECT

public:
    explicit MainPage(IFlightDataService &dataService, ITileMapService *mapFetcher, QWidget *parent = nullptr);

private:
    QWidget *buildStateGrid();
    QWidget *buildAlertPanel();
    QWidget *buildSectorDetailsPanel();

    void wireConnections();

    StateGridPanel *_gridPanel = nullptr;
    AlertPanel *_alertPanel = nullptr;
    SectorDetailsPanel *_sectorDetailsPanel = nullptr;

    ITileMapService *_mapFetcher = nullptr;
    IFlightDataService &_dataService;

public slots:
    void requestMap(double latitude, double longitude, int zoomLevel);
};