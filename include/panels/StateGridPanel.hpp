#pragma once
#include <QFrame>

#include "domain/DisplayMode.hpp"
#include "domain/SectorStates.hpp"

class QWidget;
class GridSector;
class QGridLayout;

class StateGridPanel : public QFrame
{
    Q_OBJECT

public:
    explicit StateGridPanel(int numRows, int numCols, QWidget *parent = nullptr);

    void setDisplayMode(DisplayMode mode);
    void setMapSource(const QPixmap *mapSource);

    QSize getGridSize() const;

protected:
    void resizeEvent(QResizeEvent *event) override;
    void paintEvent(QPaintEvent *event) override;

signals:
    void sectorSelected(int row, int col);

private:
    QWidget *createGrid();

    QWidget *_gridWidget;
    QGridLayout *_gridLayout;
    QPixmap _mapSource;

    GridSector *_selectedCell;

    int _numRows;
    int _numCols;

    std::vector<GridSector *> _cells;
};