#pragma once

#include <QWidget>

class StateGridPanel;
class OperatorPanel;
class SectorDetailsPanel;
class SegmentedControl;

class TileMapFetcherCARTO;

class MainPage : public QWidget
{
    Q_OBJECT

public:
    explicit MainPage(QWidget *parent = nullptr);

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


    TileMapFetcherCARTO *_mapFetcher = nullptr;

public slots:
    void requestMap(double latitude, double longitude, int zoomLevel);
};