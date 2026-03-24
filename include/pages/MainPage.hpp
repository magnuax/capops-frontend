#pragma once

#include <limits>

#include <QWidget>

class StateGridPanel;
class AlertPanel;
class SectorDetailsPanel;

class ITileMapService;
class IFlightDataService;

class MainPage : public QWidget
{
    Q_OBJECT

public slots:
    void refreshRiskEvents();
    void refresh();
    void requestMap();
    

private slots:
    void onMapFetchFailed(const QString &error);

public:
    explicit MainPage(
        IFlightDataService *dataService,
        ITileMapService *mapFetcher,
        QWidget *parent = nullptr);

private:
    void buildPage();
    QWidget *buildStateGrid();
    QWidget *buildAlertPanel();
    QWidget *buildSectorDetailsPanel();

    void wireConnections();

    StateGridPanel *_gridPanel = nullptr;
    AlertPanel *_alertPanel = nullptr;
    SectorDetailsPanel *_sectorDetailsPanel = nullptr;

    IFlightDataService *_dataService = nullptr;
    ITileMapService *_mapFetcher = nullptr;

    bool _mapLoaded = false;

    static constexpr int DETAILS_PANEL_STRETCH = 1;
    static constexpr int GRID_PANEL_STRETCH = 3;
    static constexpr int ALERT_PANEL_STRETCH = 1;
    static constexpr int MIN_GRID_WIDTH = 400;
    static constexpr int MIN_DETAILS_WIDTH = 250;
};