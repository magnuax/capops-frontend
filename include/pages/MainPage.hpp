#pragma once

#include <QWidget>

class StateGridPanel;
class AlertPanel;
class SectorDetailsPanel;
class SegmentedControl;

class IMapFetcher;

class MainPage : public QWidget
{
    Q_OBJECT

public:
    explicit MainPage(IMapFetcher *mapFetcher, QWidget *parent = nullptr);

private:
    QWidget *createStateGrid();
    QWidget *createAlertPanel();
    QWidget *createSectorDetailsPanel();
    QWidget *createDisplayControls();

    void wireConnections();

    StateGridPanel *_gridPanel = nullptr;
    SegmentedControl *_displayControls = nullptr;
    AlertPanel *_alertPanel = nullptr;
    SectorDetailsPanel *_sectorDetailsPanel = nullptr;

    IMapFetcher *_mapFetcher = nullptr;

public slots:
    void requestMap(double latitude, double longitude, int zoomLevel);
};