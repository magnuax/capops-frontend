#pragma once

#include <QWidget>

class StateGridPanel;
class OperatorPanel;
class SectorDetailsPanel;
class SegmentedControl;

class IMapFetcher;
class IFlightDataService;

class MainPage : public QWidget
{
    Q_OBJECT

public:
    explicit MainPage(IFlightDataService &dataService, IMapFetcher *mapFetcher, QWidget *parent = nullptr);

private:
    QWidget *createStateGrid();
    QWidget *createOperatorPanel();
    QWidget *createSectorDetailsPanel();
    QWidget *createDisplayControls();

    void wireConnections();

    StateGridPanel *_gridPanel = nullptr;
    SegmentedControl *_displayControls = nullptr;
    OperatorPanel *_operatorPanel = nullptr;
    SectorDetailsPanel *_sectorDetailsPanel = nullptr;

    IMapFetcher *_mapFetcher = nullptr;
    IFlightDataService &_dataService;

public slots:
    void requestMap(double latitude, double longitude, int zoomLevel);
};