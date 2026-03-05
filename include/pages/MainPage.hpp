#pragma once

#include <QWidget>

class StateGridPanel;
class OperatorPanel;
class SectorDetailsPanel;

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

    StateGridPanel *_gridPanel = nullptr;
    OperatorPanel *_operatorPanel = nullptr;
    SectorDetailsPanel *_sectorDetailsPanel = nullptr;

    TileMapFetcherCARTO *_mapFetcher = nullptr;

    void requestMap(double longitude, double latitude, int zoomLevel);
};