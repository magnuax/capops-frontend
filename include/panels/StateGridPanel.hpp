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

public:
    explicit StateGridPanel(IFlightDataService &dataService, QWidget *parent = nullptr);

    void setDisplayMode(DisplayMode mode);
    void setMapSource(const QPixmap &mapSource);

    QSize getGridSize() const;

protected:
    void resizeEvent(QResizeEvent *event) override;
    void paintEvent(QPaintEvent *event) override;

signals:
    void sectorSelected(int row, int col);

private:
    QWidget *createGrid();

    QPixmap _mapSource;

    QWidget *_gridWidget = nullptr;
    QGridLayout *_gridLayout = nullptr;
    
    int _numRows;
    int _numCols;

    IFlightDataService &_dataService;

    GridSector *_selectedCell = nullptr;
    std::vector<GridSector *> _cells;
};