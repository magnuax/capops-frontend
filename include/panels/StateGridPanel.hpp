#pragma once
#include <QFrame>

#include "domain/DisplayMode.hpp"
#include "domain/SectorStates.hpp"

class QWidget;
class GridSector;
class QGridLayout;
class IFlightDataService;

class StateGridPanel : public QFrame
{
    Q_OBJECT

signals:
    void sectorSelected(int sectorId);

public:
    explicit StateGridPanel(IFlightDataService &dataService, QWidget *parent = nullptr);

    void setDisplayMode(DisplayMode mode);

    void setMapSource(const QPixmap &mapSource);

protected:
    void resizeEvent(QResizeEvent *event) override;

private:
    QWidget *buildGrid();

    void handleSectorSelection(GridSector *cell);

    int _numRows;
    int _numCols;

    QWidget *_gridWidget = nullptr;
    QGridLayout *_gridLayout = nullptr;
    GridSector *_selectedCell = nullptr;
    std::vector<GridSector *> _cells;

    IFlightDataService &_dataService;
    QPixmap _mapSource;
};